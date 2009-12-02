#include <QDebug>

#include "sysuid.h"
#include "main.h"

#include <QTimer>
#include <QDebug>
#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>

#include "signal.h"

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
    char *argv_[] = { "sysuid", "-software" };
    argc = 2;

    DuiApplication app(argc, argv_);
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

    int ret = app.exec();
    //qDebug() << "Bye!";
    return ret;
}

