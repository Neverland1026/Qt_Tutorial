#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);

    //线程处理函数，处理复杂的操作
    void myTimeout();

    //设置线程开始与结束的标记位
    void setFlag(bool flag = true);

signals:
    void mySignal();  //这个是子线程发出的信号

public slots:

private:
    //标志位
    bool isStop;
};

#endif // MYTHREAD_H
