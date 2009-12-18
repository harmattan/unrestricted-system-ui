#include <QDebug>

#include "sysuid.h"

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
    DuiApplication app(argc, argv);
    exitPtr = &app;

    DuiApplicationWindow win;
    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    win.setWindowOpacity(0);
    win.setWindowFlags(flags);

    // qDebug and qWarning are filttered off.
    qInstallMsgHandler(0);

    signal(SIGINT, sysuid_exit);

    Sysuid daemon;

    int ret = app.exec();

    return ret;
}

