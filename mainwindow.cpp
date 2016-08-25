#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>

#include <QThread>

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //To automatically show image enable this section

    //***********************************************
    //***********************************************


    cv::Mat frame = cv::imread("vlcsnap-00060.png");

    if(!frame.data)
    {
        QMessageBox msg;
        msg.setText("Could not load image");
        msg.exec();

    }

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------

    //Las funciones de procesameinto de imagenes deben de ir aqui, la imagend de entrada es en este punto BGR
    //add the code here


    //Conver image to HSV
    cv::Mat frame_hsv;
    cv::cvtColor(frame, frame_hsv, CV_BGR2HSV);

    //creates an array of planes
    std::vector<cv::Mat> ColorPlanes;

    //Split the images into the different planes
    cv::split(frame_hsv,ColorPlanes);


    //Threshold of the image with the hue plane
    //double thresh =150;
    double maxValue =75;
    double minValue =26;
    cv::Mat threshold_image;

    cv::inRange(ColorPlanes[0],minValue, maxValue, threshold_image);

         //Create vector of contours
    std::vector<std::vector<cv::Point> > contours;

    cv::Mat Contour_image = threshold_image.clone();

    findContours(Contour_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE , cv::Point(0,0));

    std::vector<cv::Moments> mu(contours.size() );
    for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

    std::vector<cv::Point2f> mc (contours.size());
    for(int i =0 ; i< contours.size(); i++){

        mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

    }

    for(int i=0; i < mc.size(); i++){

        //floodFill(threshold_image, mc[i], cv::Scalar(255));
        cv::drawContours(threshold_image, contours, i, cv::Scalar(255), CV_FILLED, 8   );

    }

    cv::bitwise_not(threshold_image,threshold_image);

    findContours(Contour_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE , cv::Point(0,0));

    for (int i = 0; i< contours.size(); i++){

        if(cv::contourArea(contours[i]) < 500){

            cv::drawContours(threshold_image, contours, i, cv::Scalar(255), CV_FILLED, 8   );
        }

    }

    cv::bitwise_not(threshold_image,threshold_image);

    cv::Mat dst1;
    ColorPlanes[0].copyTo(dst1,threshold_image);

    cv::Mat dst2;
    cv::bitwise_not(threshold_image,threshold_image);
    dst1.copyTo(dst2,threshold_image);

    ColorPlanes[0] = dst1 + dst2;

    cv::inRange(ColorPlanes[0],150, 255, ColorPlanes[0]);

    int erosion_size =3;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1), cv::Point(erosion_size, erosion_size) );

    cv::dilate(ColorPlanes[0], ColorPlanes[0], element);

    findContours(ColorPlanes[0], contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE , cv::Point(0,0));

    cv::Rect boundRect;
    std::vector<cv::Point> contours_poly ;

    for (int i = 0; i< contours.size(); i++){

        //qDebug() << "Area " << i <<" = "<< cv::contourArea(contours[i]) ;
        if(cv::contourArea(contours[i]) > 60){

            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly, 3, true );
            boundRect = cv::boundingRect( cv::Mat(contours_poly ));
            cv::rectangle( frame, boundRect.tl(), boundRect.br(), cv::Scalar(255), 2, 8, 0 );

        }

    }


    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------

    qDebug() <<"Array size" << contours.size();
/*
    for( int i = 0; i < contours.size(); i++ )
       { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
         boundRect[i] = boundingRect( Mat(contours_poly[i]) );
         minEnclosingCircle( contours_poly[i], center[i], radius[i] );
       }



    //cv::contourArea(contours,)

    /*

    std::vector<cv::Moments> mu(contours.size() );
    for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

    std::vector<cv::Point2f> mc (contours.size());
    for(int i =0 ; i< contours.size(); i++){

        mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

    }

    */

    /*/------------------------------------------------/*
    Contour_image = threshold_image.clone();

    findContours(Contour_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE , cv::Point(0,0));

        for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

     for(int i =0 ; i< contours.size(); i++){

        mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

    }

     if ( !contours_all.empty()) {
         for (size_t i=0; i<contours.size(); ++i) {
             contour_area = contourArea(contours_all[i]) ;
             if ( contour_area < threshold)
                 small_blobs.push_back(i);
         }
     }


    for(int i=0; i < mc.size(); i++){

        //floodFill(threshold_image, mc[i], cv::Scalar(255));
        cv::drawContours(threshold_image, contours, i, cv::Scalar(10), CV_FILLED, 8   );

    }

    */

    //cv::floodFill(threshold_image, cv::Point(0,0), cv::Scalar(0));

    //cv::bitwise_not(threshold_image,threshold_image);

    /*
     *************Example to use threshold and draw a circle on an image
    cv::Mat frame_hsv;
    cv::cvtColor(frame, frame_hsv, CV_BGR2HSV);

    cv::split(frame,ColorPlanes);
    //cv::equalizeHist(ColorPlanes[0], ColorPlanes[0]);
    //cv::equalizeHist(ColorPlanes[1], ColorPlanes[1]);
    //cv::equalizeHist(ColorPlanes[2], ColorPlanes[2]);

    cv::Mat upper_red_hue_range;
    cv::Mat lower_red_hue_range;

    cv::inRange(frame_hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
    cv::inRange(frame_hsv, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

    cv::Mat red_hue_image;
    cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

    cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

    // Use the Hough transform to detect circles in the combined threshold image
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 0);

    // Loop over all detected circles and outline them on the original image
    if(circles.size() == 0) std::exit(-1);
        for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
            cv::Point center(std::ceil(circles[current_circle][0]), std::ceil(circles[current_circle][1]));
            int radius = std::ceil(circles[current_circle][2]);

            cv::circle(frame, center, radius, cv::Scalar(0, 255, 0), 5);
        }

    */
    //cv::merge(ColorPlanes,frame);

    //
    //La imagen se espera que este en BGR en este punto para que el video pueda ser reproducido
    //------------------------------------------------------------
    //------------------------------------------------------------

    cv::namedWindow("test image",cv::WINDOW_AUTOSIZE);
    cv::imshow("test image",frame);

    //***********************************************
    //***********************************************


}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    qDebug() << "Hello World";

    /*
    //***********************************************
    //***********************************************


    cv::Mat frame = cv::imread("lady.jpg");

    if(!frame.data)
    {
        QMessageBox msg;
        msg.setText("Could not load image");
        msg.exec();

    }

    //------------------------------------------------------------
    //------------------------------------------------------------

    //Las funciones de procesameinto de imagenes deben de ir aqui, la imagend de entrada es en este punto BGR
    //add the code here

    std::vector<cv::Mat> ColorPlanes;

    cv::split(frame,ColorPlanes);
    //cv::equalizeHist(ColorPlanes[0], ColorPlanes[0]);
    //cv::equalizeHist(ColorPlanes[1], ColorPlanes[1]);
    //cv::equalizeHist(ColorPlanes[2], ColorPlanes[2]);

    cv::merge(ColorPlanes,frame);

    //
    //La imagen se espera que este en BGR en este punto para que el video pueda ser reproducido
    //------------------------------------------------------------
    //------------------------------------------------------------

    cv::namedWindow("test image",cv::WINDOW_AUTOSIZE);
    cv::imshow("test image",frame);

    //***********************************************
    //***********************************************

    */



}
