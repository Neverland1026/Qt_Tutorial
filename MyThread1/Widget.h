#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <MyThread.h>  //线程头文件

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void dealTimeout();  //定时器槽函数，定时器开始后要做些什么
    void dealDone();  //线程结束槽函数，线程结束后，要做些什么
    void stopThread();  //停止线程槽函数

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTimer * myTimer;  //声明定时器
    MyThread *thread;  //声明子线程
};

#endif // WIDGET_H
