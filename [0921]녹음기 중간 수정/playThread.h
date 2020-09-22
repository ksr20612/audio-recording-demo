#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H
#include <QThread>
#include <QVariant>
#include <QObject>

class PlayThread : public QThread {
Q_OBJECT

private:
    int mNumChannel;
    int mSamplingSize;
    int mMaximumDuration;
    int mDuration;
    unsigned int mSamplingRate;
    int mFrames = 32;

public:
    PlayThread(); // constructor
    bool mIsStopped;

private:
    void run();

signals:
    void showName(QVariant);
};

#endif // PLAYTHREAD_H
