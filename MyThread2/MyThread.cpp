#include "MyThread.h"
#include <QThread>
#include <QDebug>
#include <QMessageBox>

MyThread::MyThread(QObject *parent) : QObject(parent)
{
    //线程一开始是要执行的
    isStop = false;
}

void MyThread::myTimeout(){
    while(isStop == false){
        //在这里写复杂的操作
        QThread::sleep(1);  //这里是秒

        emit mySignal();  //每次执行都发送信号

        //线程函数内部不允许操作图形界面
        //QMessageBox::aboutQt(NULL);

        qDebug()<<"子线程号 ： "<<QThread::currentThread();

        if(true == isStop){
            break;
        }
    }
}

void MyThread::setFlag(bool flag){
    isStop = flag;
}
