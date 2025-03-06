#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSerialPort *comPort;
    QString SerialPortData;
    bool isDataReceived = false;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void plot(double x, double y);
    void clear_data();
    void readData();
private slots:

    void on_stopBtn_clicked();

    void on_startBtn_clicked();

    void on_sendBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
