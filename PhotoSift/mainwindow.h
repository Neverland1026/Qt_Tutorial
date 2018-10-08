#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <MyThread.h>
#include <GPSData.h>
#include <easytools.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool isCorrected(); //检查必要项
    void dealSignalDone(vector<string> &, vector<string>& ,
                        vector<int> &,vector<int>&);  //处理从子线程传回来的数据
    void dealSignalChangeProBar(int);  //处理来自子线程传回来的进度条
    void closeWindow();

    void initUI();

signals:
    void startThread(GPSData &);

private:
    Ui::MainWindow *ui;
    MyThread * myThread;
    QThread * thread;
    QString flag;
    GPSData uidata; //将要传给子线程的数据

protected:
    void closeEvent(QCloseEvent *event);  //继承下来要实现的接口

private slots:
    void selectPath1();
    void selectPath2();
    int execPhotoSift();

    void showResult();
    void listResult();
};

#endif // MAINWINDOW_H
