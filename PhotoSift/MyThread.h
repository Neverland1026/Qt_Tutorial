#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QString>
#include <GPSData.h>
#include <easytools.h>
#include <gps.h>

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);

    int runTask();  //唯一的执行函数
    void setFlag(bool flagIsStop, bool flagIsInit); //设置标记位置

    void dealSignal(GPSData &);  //处理从主线程收到的数据

signals:

    void signalDone(vector<string> &, vector<string> &,vector<int> &,vector<int> &);  //告诉主线程运行结束
    void signalChangeUI(int);  //告诉主线程进度条数值

public slots:

private:
    bool isStop;  //标记位
    bool isInit;  //参数是否初始化
    int iPer; //进度条的值
    GPSData gpsData;

};

#endif // MYTHREAD_H
