#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>

#include <QThread>

#include <opencv2/imgproc/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    cv::Mat frame;

    qDebug() << "Hello World";

    cv::Mat test_image = cv::imread("lady.jpg");

    if(!test_image.data)
    {
        QMessageBox msg;
        msg.setText("Could not load image");
        msg.exec();

    }

    cvtColor(test_image, frame, CV_BGR2YCrCb);

    //Las funciones de procesameinto de imagenes deben de ir aqui, la imagend de entrada es en este punto YCrCb
    //add the code here

    std::vector<cv::Mat> ColorPlanes;

    cv::cvtColor(frame, frame, CV_YCrCb2BGR);

    cv::split(frame,ColorPlanes);
    //cv::equalizeHist(ColorPlanes[0], ColorPlanes[0]);
    //cv::equalizeHist(ColorPlanes[1], ColorPlanes[1]);
    //cv::equalizeHist(ColorPlanes[2], ColorPlanes[2]);

    cv::merge(ColorPlanes,frame);

    //cv::cvtColor(frame, frame, CV_BGR2BGR);
    //
    //La imagen se espera que este en BGR en este punto para que el video pueda ser reproducido
    //------------------------------------------------------------
    //------------------------------------------------------------

    cv::namedWindow("test image",cv::WINDOW_AUTOSIZE);
    cv::imshow("test image",frame);

}
