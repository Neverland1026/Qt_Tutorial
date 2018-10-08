#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MThread.h"
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getFinish(int flag);

public slots:
    void click();

signals:
    void start();

private:
    Ui::MainWindow *ui;

    MThread * mT;

    int ok;
};

#endif // MAINWINDOW_H
