#include <DuiLocale>
#include <DuiTheme>

#include <QDBusConnection>
#include <QDebug>

#include "sysuid.h"

Sysuid::Sysuid() : QObject()
{    

    qDebug() << "starting sysuidaemon";

    /* Pincode query variables */
    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("pinquery.css");
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic();

    /* Battery */
    batteryLogic = new BatteryBusinessLogic();    
    batteryLogicAdaptor = new BatteryBusinessLogicAdaptor(batteryLogic);
    if (!QDBusConnection::sessionBus().registerService("org.freedesktop.DBus.Battery")) {
        qDebug() << "failed to register dbus service";
        exit(1);
    }
    QDBusConnection::sessionBus().registerObject(QString("/"), batteryLogic);

    /* Event handler */
    eventHandler = new EventHandler();

    /* Shutdown dialog */
    shutdownLogic = new ShutdownDialogBusinessLogic();
    connect(eventHandler, SIGNAL(longPowerKeyPressOccured(bool)),
            shutdownLogic, SLOT(openDialog(bool)));
    connect(eventHandler, SIGNAL(powerKeyDownOccured()),
            shutdownLogic, SLOT(powerKeyDown()));
    connect(eventHandler, SIGNAL(powerKeyUpOccured()),
            shutdownLogic, SLOT(powerKeyUp()));

    /* Lockscreen */
    lockScreenLogic = new LockScreenBusinessLogic();
    connect(lockScreenLogic, SIGNAL(lockScreenOff()),
            batteryLogic, SLOT(initBattery()));
    connect(batteryLogic, SIGNAL(batteryCharging()),
            lockScreenLogic, SLOT(sleepModeOff()));
    connect(eventHandler, SIGNAL(shortPowerKeyPressOccured()),
            lockScreenLogic, SLOT(shortPowerKeyPressOccured()));
    connect(shutdownLogic, SIGNAL(dialogOpen(bool)),
            lockScreenLogic, SLOT(disable(bool)));

}

Sysuid::~Sysuid()
{
    delete pinCodeQueryLogic;
    pinCodeQueryLogic = NULL;
    delete batteryLogic;
    batteryLogic = NULL;
    delete lockScreenLogic;
    lockScreenLogic = NULL;

    delete shutdownLogic;
    shutdownLogic = NULL;
}
