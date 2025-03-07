#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    comPort = new QSerialPort();
    comPort->close();  // Ensure it's closed before opening
    populateSerialPorts(ui->comBox);
    populateBaudRates(ui->baudBox);
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
                ui->textEdit->append("Received Data: "+SerialPortData);
                SerialPortData = "";
                isDataReceived = false;
            }

        }
    }
}

void MainWindow::on_stopBtn_clicked()
{
    // Open the serial port
    QString currentPORT = ui->comBox->currentText();

    quint32 currentBaud = ui->baudBox->currentData().toInt();
    comPort->close();
    if (!comPort->isOpen()) {
        qDebug() << "Serial port close successfully!";
        ui->textEdit->append("Port "+currentPORT+" closed");
    } else {
        qDebug() << "Failed to open serial port:" << comPort->errorString();
    }
}


void MainWindow::on_startBtn_clicked()
{
    if (comPort->isOpen()) {
        qDebug() << " Port has opened";
        comPort->close();  // Ensure it's closed before opening
    }

    QString currentPORT = ui->comBox->currentText();

    quint32 currentBaud = ui->baudBox->currentData().toInt();

    comPort->setPortName(currentPORT);
    comPort->setBaudRate(static_cast<QSerialPort::BaudRate>(currentBaud));
    comPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    comPort->setDataBits(QSerialPort::DataBits::Data8);
    comPort->setParity(QSerialPort::Parity::NoParity);
    comPort->setStopBits(QSerialPort::StopBits::OneStop);
    // Open the serial port
    if (comPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
        ui->textEdit->append("Port "+currentPORT+" opened");
    } else {
        qDebug() << "Failed to open serial port:" << comPort->errorString();
    }
}


void MainWindow::on_sendBtn_clicked()
{
    if(comPort->isOpen())
    {
        comPort->write(ui->lineEdit->text().toLatin1() + char(10));
        comPort->flush();
    }
}

void MainWindow::populateSerialPorts(QComboBox *comPortCombo) {
    comPortCombo->clear(); // Clear existing items
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        comPortCombo->addItem(port.portName()); // Add available ports
    }
}

void MainWindow::populateBaudRates(QComboBox *baudRateCombo) {
    baudRateCombo->clear();
    QList<qint32> baudRates = QSerialPortInfo::standardBaudRates(); // Get standard baud rates
    for (qint32 baud : baudRates) {
        baudRateCombo->addItem(QString::number(baud), baud);
    }
}
