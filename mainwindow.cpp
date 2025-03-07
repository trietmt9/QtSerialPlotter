#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    comPort = new QSerialPort();
    comPort->setPortName("ttyACM0");
    comPort->setBaudRate(QSerialPort::BaudRate::Baud115200);
    comPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    comPort->setDataBits(QSerialPort::DataBits::Data8);
    comPort->setParity(QSerialPort::Parity::NoParity);
    comPort->setStopBits(QSerialPort::StopBits::OneStop);
    comPort->open(QIODevice::ReadWrite);
    if(comPort->isOpen())
    {
        qDebug() << "COM Port opened\n";
    }
    else
    {
        qDebug() << "COM Port opened failed";
        comPort->error();
    }

    connect(comPort, SIGNAL(readyRead()), this, SLOT(readData()));
    ui->plotData->addGraph(); // red line
    ui->plotData->graph(0)->setPen(QPen(QColor(255, 110, 40)));




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
            if(SerialPortData.at(SerialPortData.length() - 1) == char(10))
            {
                isDataReceived = true;
            }

            if(isDataReceived == true)
            {
                qDebug() << "Data from serial port: " << SerialPortData;
                ui->textEdit->append("Received Data: ");
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
        qDebug() << "Message sent\n";
        comPort->write(ui->lineEdit->text().toLatin1() + char(10));
        comPort->flush();
    }
}



