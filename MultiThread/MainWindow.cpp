#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mT = new MThread;

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(click()));
    connect(this,&MainWindow::start,mT,&MThread::runTask);
    connect(mT,&MThread::signalFinish,this,&MainWindow::getFinish);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete mT;
}

void MainWindow::click(){
    emit start();
}

void MainWindow::getFinish(int flag){
    qDebug()<<"over";
}
