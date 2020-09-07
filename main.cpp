#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "recorder.h"
#include <QQuickView>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine; // qml load object
    engine.load(QStringLiteral("qrc:/main.qml"));

    Recorder *recorder = new Recorder(); // qml <-> cpp object instantiated
    QObject *root = engine.rootObjects()[0]; // root objects(instance created through qml) instantiated by the engine
    recorder->setWindow(qobject_cast<QQuickWindow *>(root)); // create QQuickWindow instance & send it to "recorder"

    return app.exec();
}
