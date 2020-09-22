#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H
#include <QThread>
#include <QObject>

class RecordThread : public QThread{
Q_OBJECT

private:
    int mNumChannel = 1; // mono
    int mSamplingSize = 2; // 16bit
    int mMaximumDuration = 60;
    unsigned int mSamplingRate = 8000; // sampling rate
    int mFrames = 32;
    short *mBuffer;

public:
    RecordThread(); // constructor
    bool mIsStopped;

private:
    void run();

};

#endif // RECORDTHREAD_H
