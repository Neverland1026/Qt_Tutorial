#include "Widget.h"
#include "ui_Widget.h"
#include <QDebug>
#include <QThread>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //实例化定时器，并不是控件
    myTimer = new QTimer(this);

    //只要定时器启动。自动触发 timeout()，定时器关闭后则不执行
    connect(myTimer,&QTimer::timeout , this , &Widget::dealTimeout);

    //分配空间
    thread = new MyThread(this);

    //关联结束动作
    connect(thread, &MyThread::isDone, this, &Widget::dealDone);

    //关闭窗口右上角时，触发
    connect(this,&Widget::destroyed,this,&Widget::stopThread);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::dealTimeout(){
    static int i =0;
    i++;
    //设置lcd的值
    ui->lcdNumber->display(i);
}

void Widget::dealDone(){
    qDebug()<< "it is over";
    myTimer->stop();  //关闭定时器，
}

void Widget::stopThread(){
    //thread->terminate(); //坚决不要用，涉及到动态分配内存就会出问题
    thread->quit();  //这个会等到数据处理完，退出
    thread->wait();  //等待结束（会出现阻塞，但是感觉不到），回收资源
}

void Widget::on_pushButton_clicked()
{
    //如果定时器没有工作
    if(myTimer->isActive() == false){
        myTimer->start(100); //只执行100毫秒
    }

    //启动线程，处理数据
    thread->start();
}









