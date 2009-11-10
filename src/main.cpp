#include <QDebug>

#include "sysuid.h"
#include "main.h"

#include <QTimer>
#include <QDebug>
#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>

#include "signal.h"


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


static DuiApplication *exitPtr;
void sysuid_exit(int sig)
{
    Q_UNUSED(sig);
    if (exitPtr) {
        exitPtr->quit();
        exitPtr = NULL;
    }
}


int main(int argc, char** argv)
{    
    DuiApplication app(argc, argv);
    exitPtr = &app;

    DuiApplicationWindow win;
    Qt::WindowFlags flags = 0;
        flags |= Qt::FramelessWindowHint;// | Qt::WindowStaysOnTopHint;
    win.setWindowOpacity(0);
    win.setWindowFlags(flags);

    // qDebug and qWarning are filttered off.
    qInstallMsgHandler(0);

    signal(SIGINT, sysuid_exit);

    Sysuid daemon;

    /* Comment in if rotation test needed. *
    TestObj obj;
    QTimer *rotation = new QTimer(0);
    rotation->setInterval(5*1000);
    rotation->start();
    QObject::connect(rotation, SIGNAL(timeout()), &obj, SLOT(doRotation()));
    **/

    int ret = app.exec();
    //qDebug() << "Bye!";
    return ret;
}

