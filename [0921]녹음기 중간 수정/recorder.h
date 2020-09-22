#ifndef RECORDER_H
#define RECORDER_H

#include <stdio.h>
#include <QObject>

class Recorder : public QObject
{
Q_OBJECT

private:
    int mNumChannel; // mono
    int mSamplingSize; // 16bit
    int mMaximumDuration;
    int mDuration;
    unsigned int mSamplingRate; // sampling rate
    int mFrames;
    short *mBuffer = new short[mMaximumDuration*mSamplingRate*mNumChannel];
    bool mIsRecorded = false;

public slots:
    void startRecording(QVariant intData);

};

#endif // RECORDER_H
