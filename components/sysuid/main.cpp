#include "sysuid.h"

#include <QTimer>
#include <QDebug>
#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>

#include <QObject>
#include <QDebug>

#include "signal.h"

static DuiApplication *exitPtr;

void sysuid_exit (int sig)
{
    Q_UNUSED (sig);
    if (exitPtr) {
        exitPtr->quit ();
        exitPtr = NULL;
    }
}

int main (int argc, char** argv)
{
    DuiApplication app (argc, argv);
    exitPtr = &app;

    DuiApplicationWindow win;
    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    win.setWindowOpacity (0);
    win.setWindowFlags (flags);

    qDebug () << "Starting system-ui";
    // qDebug and qWarning are filttered off.
    qInstallMsgHandler (0);

    qDebug () << "Starting system-ui 2";

    signal (SIGINT, sysuid_exit);

    Sysuid daemon;

    qDebug () << "- System-UI daemon initialized";

    // re-install the translations on locale settings changed signal
    QObject::connect (&app, SIGNAL (localeSettingsChanged ()),
                      &daemon, SLOT (retranslate ()));

    int ret = app.exec ();

    return ret;
}

