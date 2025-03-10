#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <qcustomplot.h>
#include <QTime>
#include <QTimer>
#include <QRandomGenerator>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    bool isDataReceived = false;
    bool comState = false;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void plot(double x, double y);
    void clear_data();

private slots:

    void on_sendBtn_clicked();

    void readData();

    void populateSerialPorts(QComboBox *comPortCombo);

    void populateBaudRates(QComboBox *baudRateCombo);


    void on_runBtn_clicked();

    void on_pauseBtn_clicked();

    void on_exportBtn_clicked();

    void on_connectBtn_clicked();

    void on_disBtn_clicked();

    void on_clearBtn_clicked();

    void on_refreshBtn_clicked();

private:
    Ui::MainWindow *ui;
    QCustomPlot *plotData;
    QSerialPort *comPort;
    QString SerialPortData;
    QTime startTime;    // Tracks the start time
    QTimer dataTimer;   // Timer for plotting or real-time updates
};
#endif // MAINWINDOW_H
