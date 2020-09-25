#ifndef COUNTTHREAD_H
#define COUNTTHREAD_H

#include <QVariant>
#include <QThread>
#include <QObject>

class CountThread : public QThread{
Q_OBJECT

private:
    void run();

public:
    bool mIsStopped = false;

signals:
    void showCount(QVariant second);

};

#endif // COUNTTHREAD_H
