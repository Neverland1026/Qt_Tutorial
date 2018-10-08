#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>  //添加头文件

class MyThread : public QThread  //修改基类
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);

protected:
    void run(); //虚函数，线程处理函数，不能直接调用通过start()间接调用

signals:
    void isDone(); //发出信号线程是否结束了，只声明不实现

public slots:

};

#endif // MYTHREAD_H
