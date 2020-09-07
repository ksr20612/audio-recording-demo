#ifndef RECORDER_H
#define RECORDER_H

#include <QQuickView>
#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

class Recorder : public QObject
{
Q_OBJECT

private:
    QQuickWindow *mMainView;
    int mNumChannel = 1; // mono
    int mSamplingSize = 2; // 16bit
    int mMaximumDuration = 60;
    int mDuration;
    unsigned int mSamplingRate = 8000; // sampling rate
    int mFrames = 32;
    short *mBuffer = new short[mMaximumDuration*mSamplingRate*mNumChannel];
    bool mIsRecorded = false;

public:
    void connectQmlCpp(); // connect qml&cpp
    void setWindow(QQuickWindow* window); // mMainView(qml instance) initialization

public slots:
    void startRecording(QVariant intData);
    void startPlaying(QVariant intData);
};

#endif // RECORDER_H
