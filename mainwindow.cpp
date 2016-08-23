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

    qDebug() << "Hello World";

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

}
