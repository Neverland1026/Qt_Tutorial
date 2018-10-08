#include "Widget.h"
#include "ui_Widget.h"
#include <QDebug>
#include <Qt>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    myT = new MyThread;  //分配空间，不能指定父对象
    thread = new QThread(this);   //创建子线程
    //把我自定义的线程加入到子线程中
    //如果 myT 指定了父对象，那么的话系统不允许你把有父亲的儿子移动到别的父亲
    myT->moveToThread(thread);

    //运行中的子线程发出了信号，UI线程接收信号（用于更新界面UI，这里是更新的lcdNumber）
    connect(myT,&MyThread::mySignal,this,&Widget::dealSignal);

    //UI线程发出启动子线程信号，子线程接收到信号后开始执行复杂运算
    //第五个参数的使用 Qt::QueuedConnection  //Qt::DirectConnection
    connect(this,&Widget::startThread,myT,&MyThread::myTimeout);

    //UI线程发出的关闭窗口信号，UI线程做子线程善后工作
    connect(this,&Widget::destroyed,this,&Widget::dealDestroy);

    //打印UI线程句柄
    qDebug()<<"主线程号 ： "<<QThread::currentThread();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::dealSignal(){  //改变UI界面
    static int i=0;
    i++;
    ui->lcdNumber->display(i);
}

void Widget::dealDestroy(){
    on_pushButton_2_clicked();
    delete myT;  //不是必须的，不写可能会导致内存泄漏
}

void Widget::on_pushButton_clicked()
{
    if(thread->isRunning() == true)
        return;

    //启动线程，但是没有启动线程处理函数
    thread->start();
    myT->setFlag(false);

    //不能直接调用线程处理函数，直接调用导致线程处理函数和主线程在同一个线程
    //myT->myTimeout();

    //只能通过 signal - slot 调用
    emit startThread();
}

void Widget::on_pushButton_2_clicked()
{
    if(thread->isRunning() == false)
        return;
    //这种做法实际上停止不了线程
    myT->setFlag(true);
    thread->quit();
    thread->wait();
}

#if 0
1、线程函数内部不允许操作图形界面
2、connect() 的第五个参数怎么用
    第五个参数只有在多线程的情况下才有意义
        如果是多线程，默认使用队列
        如果是单线程，默认使用直接方式
    队列：槽函数所在的线程和信号接收者一样
    直接：槽函数所在的线程和发送者一样
3、UI线程负责发出2个信号：1是启动子线程的信号；2是关闭窗口结束进程的信号
4、子线程只负责在运行发出信号，不运行不发信号
#endif
