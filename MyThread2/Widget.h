#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <MyThread.h>  //
#include <QThread>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void dealSignal();   //处理来自子线程的信号
    void dealDestroy();  //处理关闭窗口时间，主要用来结束线程

private slots:
    void on_pushButton_clicked();//start按钮的操作 --- 开始线程
    void on_pushButton_2_clicked();//stop 按钮的操作 --- 结束线程

signals:
    void startThread();  //启动子线程的信号

private:
    Ui::Widget *ui;  //UI 界面
    MyThread *myT;   //线程对象
    QThread *thread;  //子线程
};

#endif // WIDGET_H
