#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;  //这就是展示的主窗口

    //注册数据类型（这句话太他妈重要了）
    qRegisterMetaType<GPSData>("GPSData");  //变量类型
    qRegisterMetaType<GPSData>("GPSData&");  //引用类型
    qRegisterMetaType<vector<string>>("vector<string>");  //变量类型
    qRegisterMetaType<vector<string>>("vector<string>&");  //引用类型
    qRegisterMetaType<vector<int>>("vector<int>");  //变量类型
    qRegisterMetaType<vector<int>>("vector<int>&");  //引用类型

    //设置窗口标题和图标
    w.setWindowTitle("PhotoSift(Neverland_LY@163.com)");
    w.setWindowIcon(QIcon(QStringLiteral(":icon/app_icon.png")));

    //设置无边框
    //w.setWindowFlags(Qt::FramelessWindowHint);

    //设置窗体居中
    w.move((QApplication::desktop()->width() - w.width())/2,
           (QApplication::desktop()->height() - w.height())/2);

    //让窗口一直置顶
    w.setWindowFlags(Qt::WindowStaysOnTopHint);
    w.show();

    return a.exec();
}








