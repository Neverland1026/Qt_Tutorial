#ifndef MTHREAD_H
#define MTHREAD_H

#include <QObject>
#include <QThread>

class MThread : public QObject
{
    Q_OBJECT
public:
    explicit MThread(QObject *parent = 0);

    void runTask();

signals:

    void signalFinish(int flag);


private:
    QThread thread;


};

#endif // MTHREAD_H
