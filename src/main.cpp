#include <DuiApplicationWindow>
#include "sysuid.h"
#include "main.h"

#include <QTimer>
#include <DuiSceneManager>
#include <DuiApplication>


TestObj::TestObj() : QObject()
{
}

TestObj::~TestObj()
{
}

void TestObj::doRotation()
{
    int orientation = (int)DuiSceneManager::instance()->orientationAngle();
    orientation += 90;
    orientation %= 360;
    DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) orientation);
}

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);

    DuiApplicationWindow win;
    Qt::WindowFlags flags = 0;
        flags |= Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
    win.setWindowOpacity(0);
    win.setWindowFlags(flags);

    // qDebug and qWarning are filttered off.
    qInstallMsgHandler(0);

    Sysuid daemon(win);

    TestObj obj;
    QTimer *rotation = new QTimer(0);
    rotation->setInterval(5*1000);
    rotation->start();
    //QObject::connect(rotation, SIGNAL(timeout()), &obj, SLOT(doRotation()));

    return app.exec();
}

