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

    QObject::connect(mMainView, SIGNAL(startRecord(QVariant,QVariant)),this,SLOT(recordClicked(QVariant,QVariant))); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(startPlay(QVariant)),this,SLOT(playClicked(QVariant))); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(stopRecord()),this,SLOT(recordStopped())); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(stopPlay()),this,SLOT(playStopped())); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(conveyName(QVariant)),this,SLOT(nameConveyed(QVariant)));
    QObject::connect(mMainView,SIGNAL(conveyNameforPlay(QVariant)),this,SLOT(nameConveyedforPlay(QVariant)));

}

void ConnectEvent::setWindow(QQuickWindow* Window){

    mMainView = Window;
    connectQmlCpp();
}

void ConnectEvent::recordClicked(QVariant chan, QVariant rate){

    int channel = chan.toInt();
    int sampleRate = rate.toInt();
    cout << "channel : " << channel << endl;
    cout << "rate : " << sampleRate << endl;
    rt = new RecordThread(channel,sampleRate);
    ct = new CountThread();
    QObject::connect(ct,SIGNAL(showCount(QVariant)),mMainView,SLOT(showCount(QVariant)));
    rt->start();
    ct->start();

}

void ConnectEvent::playClicked(QVariant name){

    pt = new PlayThread();

    // name transforming
    string fileName = name.toString().toStdString();
    strcpy(pt->mFileName,fileName.c_str());

    QObject::connect(pt,SIGNAL(showName(QVariant)),mMainView,SLOT(showName(QVariant)));
    QObject::connect(pt,SIGNAL(loadFail()),mMainView,SLOT(loadFail()));
    QObject::connect(pt,SIGNAL(closeName()),mMainView,SLOT(closeName()));
    pt->start();
}

void ConnectEvent::recordStopped(){

    cout << "recordStop" << endl;
    rt->mIsStopped = true;
    ct->mIsStopped = true;
}

void ConnectEvent::playStopped(){

    pt->mIsStopped = true;

}

void ConnectEvent::nameConveyed(QVariant name){

    string fileName = name.toString().toStdString();
    strcpy(rt->mfileName,fileName.c_str());

}

void ConnectEvent::nameConveyedforPlay(QVariant name){

    string fileName = name.toString().toStdString();
    strcpy(pt->mFileName,fileName.c_str());

}
