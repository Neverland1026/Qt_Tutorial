#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QRadioButton>
#include <QMessageBox>
#include <QRegExp>
#include <QValidator>
#include <QIcon>
#include <QDebug>
#include <QMutexLocker>
#include <QSize>
#include <QCloseEvent>
#include <Qt>
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置窗口图标
    this->setWindowIcon(QIcon(QStringLiteral(":icon/app_icon.png")));

    //初始化线程
    myThread = new MyThread;
    thread = new QThread(this);
    myThread->moveToThread(thread);

    //初始化界面
    initUI();

    //关联槽函数，UI自身的槽函数
    connect(ui->tbtnSel1,SIGNAL(clicked()),this,SLOT(selectPath1()));
    connect(ui->tbtnSel2,SIGNAL(clicked()),this,SLOT(selectPath2()));
    connect(ui->btnExec,SIGNAL(clicked()),this,SLOT(execPhotoSift()));
    connect(ui->btnShow,SIGNAL(clicked()),this,SLOT(showResult()));
    connect(ui->btnList,SIGNAL(clicked()),this,SLOT(listResult()));

    //与线程有关的关联
    //主线程发出开始子线程的命令，子线程开始处理传递过来的参数
    connect(this,&MainWindow::startThread,myThread,&MyThread::dealSignal);
    //主线程发出开始子线程的命令，子线程开始执行复杂操作
    connect(this,&MainWindow::startThread,myThread,&MyThread::runTask);
    //主线程发出窗口关闭事件，主线程响应
    //connect(this,&MainWindow::destroyed,this,&MainWindow::closeWindow);
    //子线程返回执行完毕的信号，主线程停止子线程的运行
    connect(myThread,&MyThread::signalDone,this,&MainWindow::dealSignalDone);
    //子线程发来改变进度条的信号，UI线程收到后开始处理信号
    connect(myThread,&MyThread::signalChangeUI,this,&MainWindow::dealSignalChangeProBar);
    //跟踪tablewidget的行
    //connect(ui->tableWidget,SIGNAL(cellEntered()),);
    //打印UI线程句柄
    qDebug()<<"主线程号 ： "<<QThread::currentThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI(){

    //设置窗口参数
    this->setWindowIcon(QIcon("app_icon.ico")); //设置icon
    //this->setMaximumSize(700,400); //设置窗口的大小
    //this->setMinimumSize(700,400);
    this->setFixedSize(700,400);
    //this->move(0,0);  //固定位置

    //设置初始化位置
    ui->groupBox->setGeometry(10,15,680,110);
    ui->label->setGeometry(20,20,100,50);
    ui->label_2->setGeometry(20,60,100,50);
    ui->lEdit1->setGeometry(90,33,500,20);
    ui->lEdit2->setGeometry(90,73,500,20);
    ui->tbtnSel1->setGeometry(600,32,70,22);
    ui->tbtnSel2->setGeometry(600,72,70,22);    //GroupBox1

    ui->groupBox_2->setGeometry(10,130,680,110);
    ui->label_3->setGeometry(20,20,100,50);
    ui->label_4->setGeometry(20,60,100,50);
    ui->editMaxLon->setGeometry(90,34,220,20);
    ui->editMinLon->setGeometry(90,74,220,20);
    ui->label_5->setGeometry(350,20,100,50);
    ui->label_6->setGeometry(350,60,100,50);
    ui->editMaxLat->setGeometry(420,34,220,20);
    ui->editMinLat->setGeometry(420,74,220,20);   //GroupBox2

    ui->groupBox_3->setGeometry(10,245,680,70);
    ui->rbCopy->setGeometry(50,30,70,20);
    ui->rbPaste->setGeometry(310,30,70,20);
    ui->rbDelete->setGeometry(560,30,70,20);     //GroupBox3

    ui->proBar->setGeometry(10,330,675,20);    //进度条
    ui->btnExec->setGeometry(300,360,100,30);   //确定按钮
    ui->btnShow->setGeometry(580,360,100,30);   //展示结果
    ui->btnList->setGeometry(580,360,100,30);   //收起结果

    ui->tableWidget->setGeometry(10,400,680,540);  //设置 tablewidget

    //设置UI
    ui->rbCopy->setChecked(true);  //默认设置为 copy 复制
    ui->proBar->setValue(0);  //设置进度条的初始位置及状态
    ui->proBar->setVisible(false);  //设置进度条的不可见
    ui->btnShow->setVisible(false);
    ui->btnList->setVisible(false);
    //ui->btnShow->setFocusPolicy(Qt::NoFocus);  //这个是取消focus状态的虚线框
    //ui->btnList->setFocusPolicy(Qt::NoFocus);

    //设置tablewidget
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //ui->tableWidget->verticalHeader()->setVisible(false);
    //ui->tableWidget->horizontalHeader()->setVisible(false);
    QStringList HStrList;
    HStrList.push_back(QString::fromLocal8Bit("图片号"));
    HStrList.push_back(QString("执行的操作"));
    HStrList.push_back(QString("状态"));
    //设置行列数(只有列存在的前提下，才可以设置列标签)
    int HlableCnt = HStrList.count();
    ui->tableWidget->setColumnCount(HlableCnt);

    //设置列宽
    ui->tableWidget->setColumnWidth(0,250);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui->tableWidget->setColumnWidth(2,240);
    //设置每行选中颜色
    //ui->tableWidget->setStyleSheet("selection-background-color:rgb(255,250,205)");

    //设置列标签
    ui->tableWidget->setHorizontalHeaderLabels(HStrList);
    //ui->tableWidget->resizeColumnsToContents();  //行列大小与内容相匹配
    //ui->tableWidget->resizeRowsToContents();
    //ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置鼠标跟踪
    ui->tableWidget->setMouseTracking(true);

    ui->btnExec->setFocus();

    //设置经度输入范围---限制浮点数输入范围为[-180,180]并限定为小数位后7位
    QRegExp regx("^-?(180|([1-9]?[0-9]|1[0-7][0-9])(\\.\\d{1,7})?)$");
    QValidator *validator1 = new QRegExpValidator(regx, ui->editMaxLon);
    ui->editMaxLon->setValidator( validator1 );
    QValidator *validator2 = new QRegExpValidator(regx, ui->editMinLon);
    ui->editMinLon->setValidator( validator2 );
    //设置纬度输入范围---限制浮点数输入范围为[-90,90]并限定为小数位后7位
    QRegExp regx2("^-?(90|[1-8]?\\d(\\.\\d{1,7})?)$");
    QValidator *validator3 = new QRegExpValidator(regx2, ui->editMaxLat);
    ui->editMaxLat->setValidator( validator3 );
    QValidator *validator4 = new QRegExpValidator(regx2, ui->editMinLat);
    ui->editMinLat->setValidator( validator4 );
}

void MainWindow::dealSignalDone(vector<string> & files, vector<string>& infos,
                                vector<int> &failedFiles,vector<int>&occupiedFiles){
    qDebug(">>> 主线程收到来自子线程运行结束的信号！！！");

    myThread->setFlag(true,false);
    thread->quit();
    thread->wait();
    ui->btnExec->setEnabled(true);
    ui->rbCopy->setEnabled(true);
    ui->rbPaste->setEnabled(true);
    ui->rbDelete->setEnabled(true);
    ui->btnExec->setText("开始");
    ui->editMaxLon->setReadOnly(false);
    ui->editMinLon->setReadOnly(false);
    ui->editMaxLat->setReadOnly(false);
    ui->editMinLat->setReadOnly(false);
    ui->proBar->setValue(100);  //进度条强制 100%
    ui->btnShow->setVisible(true);
    ui->btnShow->setVisible(false);
    ui->btnList->setVisible(false);

    //开始填充单元格
    //这部分需要开始新线程，暂不执行
    /*
    ui->tableWidget->setRowCount(files.size());
    for(int i = 0; i < files.size() ; i++){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(s2q(files[i])));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(flag));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem("成功"));
    }

    for(int j = 0 ; j < failedFiles.size() ; j++){
        ui->tableWidget->item(j,2)->setTextColor(QColor(255,0,0));
        ui->tableWidget->setItem(j,2,new QTableWidgetItem("失败"));
    }

    for(int k = 0 ; k < occupiedFiles.size() ; k++){
        ui->tableWidget->item(k,2)->setTextColor(QColor(255,0,0));
        ui->tableWidget->setItem(k,2,new QTableWidgetItem("文件被占用或删除，操作失败"));
    }
    */

}

void MainWindow::dealSignalChangeProBar(int per){
    //qDebug(">>>>>> 子线程 -> UI 线程 ： %d",per);
    ui->proBar->setValue(per);
}

void MainWindow::closeWindow(){
    if(thread->isRunning() == true){
        QMessageBox::critical(this,"警告","任务正在进行，请勿关闭窗口！！！");
        return;
    }

    myThread->setFlag(true,false);
    thread->quit();
    thread->wait();
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(thread->isRunning() == true){
        QMessageBox box(QMessageBox::NoIcon, "警告", "任务正在进行，你确认要退出吗？",
                        QMessageBox::Yes | QMessageBox::No, NULL);
        box.setButtonText(QMessageBox::Yes, QString("确定"));
        box.setButtonText(QMessageBox::No, QString("取消"));
        if(box.exec() == QMessageBox::Yes){
            myThread->setFlag(true,false);
            thread->quit();
            thread->wait();
            event->accept();
        }else
            event->ignore();
    }else
        event->accept();
}

void MainWindow::selectPath1(){
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"),
                "/home",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lEdit1->setText(dir);
    ui->lEdit1->setReadOnly(true);
}

void MainWindow::selectPath2(){
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"),
                "/home",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lEdit2->setText(dir);
    ui->lEdit2->setReadOnly(true);
}

bool MainWindow::isCorrected(){
    //第一步：检查是否都选择正确路径
    if(q2s(ui->lEdit1->text()) == ""){
        QMessageBox::critical(this,"警告","请选择文件路径！！！");
        return false;
    }

    if(q2s(ui->lEdit2->text()) == ""){
        QMessageBox::critical(this,"警告","请选择文件路径！！！");
        return false;
    }

    //第二步：获取最大经纬度数据并进行检查，检查通过设置为只读
    QString arr[4] = {ui->editMaxLon->text(),
                      ui->editMinLon->text(),
                      ui->editMaxLat->text(),
                      ui->editMinLat->text()};

    for(int i = 0; i <sizeof(arr)/sizeof(arr[0]);i++)
        if(q2s(arr[i]) == ""){
            QMessageBox::critical(this,"警告","经纬度输入不能为空！！！");
            return false;
        }

    ui->editMaxLon->setReadOnly(true);
    ui->editMinLon->setReadOnly(true);
    ui->editMaxLat->setReadOnly(true);
    ui->editMinLat->setReadOnly(true);

    //第三步：锁定radiobutton状态
    if(ui->rbCopy->isChecked()){
        flag = "COPY";
    }else if(ui->rbPaste->isChecked()){
        flag = "PASTE";
    }else{
        flag = "DELETE";
    }
    ui->btnShow->setVisible(false);
    ui->rbCopy->setEnabled(false);
    ui->rbPaste->setEnabled(false);
    ui->rbDelete->setEnabled(false);

    //第四步：显示进度条
    ui->proBar->setVisible(true);

    return true;
}

int MainWindow::execPhotoSift(){

    if(thread->isRunning()  == true)
        return -1;

    if(!isCorrected())
        return -1;

    ui->btnExec->setEnabled(false);
    ui->btnExec->setText("运行中...");
    ui->btnShow->setFocus();

    //关键操作
    myThread->setFlag(false,false);
    thread->start();

    GPSData d(ui->lEdit1->text(),ui->lEdit2->text(),
              ui->editMaxLon->text(),ui->editMinLon->text(),
              ui->editMaxLat->text(),ui->editMinLat->text(),flag);

    uidata = d;
    emit startThread(uidata);   //这里就要传参了，即传递初始化数据

    return 0;
}

void MainWindow::showResult(){

    this->setFixedSize(700,950);
    ui->btnShow->setVisible(false);
    ui->btnList->setVisible(true);
}

void MainWindow::listResult(){
    this->setFixedSize(700,400);
    ui->btnShow->setVisible(true);
    ui->btnList->setVisible(false);
}








