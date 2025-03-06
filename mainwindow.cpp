#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plotData->addGraph(); // red line
    ui->plotData->graph(0)->setPen(QPen(QColor(255, 110, 40)));
    comPort = new QSerialPort();
    connect(comPort, SIGNAL(readyRead()), this, SLOT(readData));


};

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plot(double x, double y)
{

}

void MainWindow::clear_data()
{

}

void MainWindow::readData()
{
    if(comPort->isOpen())
    {
        while(comPort->bytesAvailable())
        {
            SerialPortData += comPort->readAll();
            if(SerialPortData.at(SerialPortData.length() - 1)== char(10))
            {
                isDataReceived = true;
            }

            if(isDataReceived == true)
            {
                qDebug() << "Data from serial port: " << SerialPortData;
                ui->textEdit->append(SerialPortData);
                SerialPortData = "";
                isDataReceived = false;
            }

        }
    }
}

void MainWindow::on_stopBtn_clicked()
{

}


void MainWindow::on_startBtn_clicked()
{

}


void MainWindow::on_sendBtn_clicked()
{
    if(comPort->isOpen())
    {
        comPort->write(ui->lineEdit->text().toLatin1() + char(10));
        comPort->flush();
    }
}



