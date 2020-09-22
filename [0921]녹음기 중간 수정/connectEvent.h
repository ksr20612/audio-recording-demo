#ifndef CONNECTEVENT_H
#define CONNECTEVENT_H

#include <QQuickView>
#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include "playThread.h"
#include "recordThread.h"
#include "countThread.h"

class ConnectEvent : public QObject
{
Q_OBJECT

private:
    QQuickWindow *mMainView;

    RecordThread *rt;
    PlayThread *pt;
    CountThread *ct;

public:
    void connectQmlCpp(); // connect qml&cpp
    void setWindow(QQuickWindow* window); // mMainView(qml instance) initialization

public slots:
    void recordClicked(QVariant intData);
    void recordStopped();
    void playClicked(QVariant intData);
    void playStopped();

};

#endif // CONNECTEVENT_H
