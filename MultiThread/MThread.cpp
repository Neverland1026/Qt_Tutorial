#include "MThread.h"
#include "windows.h"

#include <QDebug>
#include <QMetaObject>

MThread::MThread(QObject *parent) : QObject(parent)
{
    QMetaObject::invokeMethod(,)
}

void MThread::runTask(){
    qDebug()<<"task is starting";

    this->moveToThread(&thread);
    thread.start();

    Sleep(5000);

    thread.quit();
    thread.wait();

    emit signalFinish(2);
}
