#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , startTime(QTime::currentTime()) // We'll use this to track elapsed time
{
    ui->setupUi(this);
    comPort = new QSerialPort();
    comPort->close();  // Ensure it's closed before opening
    populateSerialPorts(ui->comBox);
    populateBaudRates(ui->baudBox);
    connect(comPort, SIGNAL(readyRead()),
            this, SLOT(readData()));
    // 1) Add two graphs: a blue line and a red line
    ui->plotData->addGraph(); // Graph 0
    ui->plotData->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->plotData->addGraph(); // Graph 1
    ui->plotData->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    // 2) Use a time ticker on the x-axis
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotData->xAxis->setTicker(timeTicker);

    // 3) Configure axes
    ui->plotData->axisRect()->setupFullAxesBox();
    ui->plotData->yAxis->setRange(-5, 5);

    // 4) Make left/bottom axes transfer their ranges to right/top axes
    connect(ui->plotData->xAxis,
            QOverload<const QCPRange &>::of(&QCPAxis::rangeChanged),
            ui->plotData->xAxis2,
            QOverload<const QCPRange &>::of(&QCPAxis::setRange));

    connect(ui->plotData->yAxis,
            QOverload<const QCPRange &>::of(&QCPAxis::rangeChanged),
            ui->plotData->yAxis2,
            QOverload<const QCPRange &>::of(&QCPAxis::setRange));
    // 5) Setup the timer that calls realtimeDataSlot
    connect(&dataTimer, &QTimer::timeout, this, &MainWindow::on_runBtn_clicked);
    dataTimer.start(0); // Interval 0 = refresh as fast as possible

}

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

void MainWindow::on_disBtn_clicked()
{
    // Open the serial port
    QString currentPORT = ui->comBox->currentText();

    quint32 currentBaud = ui->baudBox->currentData().toInt();
    comPort->close();
    if(comState == true)
    {
        if (!comPort->isOpen()) {
            qDebug() << "Serial port close successfully!";
            ui->textEdit->append("Port "+currentPORT+" closed");
        } else {
            qDebug() << "Failed to open serial port:" << comPort->errorString();
        }
    }
    else
    {
        qDebug() << "Port not found"<< comPort->errorString();
        ui->textEdit->append(comPort->errorString());
    }
}


void MainWindow::on_connectBtn_clicked()
{
    if (comPort->isOpen()) {
        qDebug() << " Port has opened";
        comState = true;
        // comPort->close();  // Ensure it's closed before opening
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
        comState = true;
    } else{
        qDebug() << comPort->errorString();
        ui->textEdit->append(comPort->errorString());
        comState = false;
    }
}


void MainWindow::on_sendBtn_clicked()
{
    if(comPort->isOpen())
    {
        comPort->write(ui->inputLine->text().toLatin1() + char(10));
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



void MainWindow::on_runBtn_clicked()
{
    QElapsedTimer elapsedTimer;
    elapsedTimer.start();
    // calculate two new data points:
    double key = startTime.msecsTo(QTime::currentTime()) / 1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
        // add data to lines:
        ui->plotData->graph(0)->addData(key, qSin(key)+QRandomGenerator::global()->generate()/(double)RAND_MAX*1*qSin(key/0.3843));
        ui->plotData->graph(1)->addData(key, qCos(key)+QRandomGenerator::global()->generate()/(double)RAND_MAX*0.5*qSin(key/0.4364));
        // rescale value (vertical) axis to fit the current data:
        //ui->plotData->graph(0)->rescaleValueAxis();
        //ui->plotData->graph(1)->rescaleValueAxis(true);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotData->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plotData->replot();

}


void MainWindow::on_pauseBtn_clicked()
{

}


void MainWindow::on_exportBtn_clicked()
{

}



void MainWindow::on_clearBtn_clicked()
{
    ui->textEdit->clear();
}


void MainWindow::on_refreshBtn_clicked()
{
    populateSerialPorts(ui->comBox);
}

