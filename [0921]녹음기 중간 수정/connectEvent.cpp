#include "connectEvent.h"
#include "recorder.h"
#include "player.h"
#include "wavWriter.h"
#include "wavReader.h"
#include <iostream>
#include <alsa/asoundlib.h>
#include <string.h>
#include <stdio.h>
#include <QCoreApplication>
#include <thread>
#include <QVariant>
using namespace std;

void ConnectEvent::connectQmlCpp(){

    // play - stop button
    // capture - play segmentation
    // parameters all conveyed needed.
    // file inserting clean needed

    QObject::connect(mMainView, SIGNAL(startRecord(QVariant)),this,SLOT(recordClicked(QVariant))); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(startPlay(QVariant)),this,SLOT(playClicked(QVariant))); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(stopRecord()),this,SLOT(recordStopped())); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(stopPlay()),this,SLOT(playStopped())); // qml -> cpp

}

void ConnectEvent::setWindow(QQuickWindow* Window){

    mMainView = Window;
    connectQmlCpp();
}

void ConnectEvent::recordClicked(QVariant intData){

    if(intData==1) cout << "YOU CAN START RECORDING" << endl;
    rt = new RecordThread();
    ct = new CountThread();
    QObject::connect(ct,SIGNAL(showCount(QVariant)),mMainView,SLOT(showCount(QVariant)));
    rt->start();
    ct->start();

}

void ConnectEvent::playClicked(QVariant intdata){

    if(intdata == 1) cout << "YOU CAN START PLAYING" << endl;
    pt = new PlayThread();
    QObject::connect(pt,SIGNAL(showName(QVariant)),mMainView,SLOT(showName(QVariant)));
    pt->start();

}

void ConnectEvent::recordStopped(){

    cout << "recordStop" << endl;
    rt->mIsStopped = true;
    ct->mIsStopped = true;
}

void ConnectEvent::playStopped(){

    pt->mIsStopped = true;
    cout << "playStop" << endl;

}

