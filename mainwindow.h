#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <qcustomplot.h>

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

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void plot(double x, double y);
    void clear_data();

private slots:

    void on_stopBtn_clicked();

    void on_startBtn_clicked();

    void on_sendBtn_clicked();

    void readData();
private:
    Ui::MainWindow *ui;
    QSerialPort *comPort;
    QString SerialPortData;
};
#endif // MAINWINDOW_H
