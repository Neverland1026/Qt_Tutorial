#include "MyThread.h"

MyThread::MyThread(QObject *parent) : QThread(parent)  //记得修改基类
{

}

void MyThread::run(){
    //很复杂的数据处理需要耗时5s
    sleep(5);  //这是以秒为单位 ，加 m 是毫秒，加 u 是微秒
    emit isDone(); //最后一刻再释放信号
}
