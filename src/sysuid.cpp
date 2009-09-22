#include <DuiLocale>
#include <DuiTheme>

#include <QDBusConnection>
#include <QDebug>

#include "sysuid.h"

Sysuid::Sysuid() : QObject()
{    
    qDebug() << "starting sysuidaemon";

    /* GConf interface */
    systemUIGConf = new SystemUIGConf();

    /* Pincode query variables */
    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("pinquery.css");
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic();

    /* Battery */         
    batteryLogic = new BatteryBusinessLogic(systemUIGConf);
    batteryLogicAdaptor = new BatteryBusinessLogicAdaptor(batteryLogic);    
    registerDBusServiceAndObject(QString("org.freedesktop.DBus.Battery"), QString("/systemui/battery"), static_cast<QObject*>(batteryLogic));
    // TODO: how to handle the unseccessfull registering

    /* Display */
    displayLogic = new DisplayBusinessLogic(systemUIGConf);
    displayLogicAdaptor = new DisplayBusinessLogicAdaptor(displayLogic);    
    registerDBusServiceAndObject(QString("org.freedesktop.DBus.Display"), QString("/systemui/display"), static_cast<QObject*>(displayLogic));    
    // TODO: how to handle the unseccessfull registering

    /* Event handler */
    eventHandler = new EventHandler();

    /* Shutdown */
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

bool Sysuid::registerDBusServiceAndObject(const QString &service, const QString &objectPath, QObject *object)
{
    if(!QDBusConnection::sessionBus().registerService(service)) {
        qDebug() << "failed to register dbus service";
        return false;
    }
    if(!QDBusConnection::sessionBus().registerObject(objectPath, object)) {
        qDebug() << "failed to register dbus object";
        return false;
    }
    return true;
}

Sysuid::~Sysuid()
{
    delete systemUIGConf;
    systemUIGConf = NULL;
    delete pinCodeQueryLogic;
    pinCodeQueryLogic = NULL;
    delete batteryLogic;
    batteryLogic = NULL;
    delete batteryLogicAdaptor;
    batteryLogicAdaptor = NULL;
    delete displayLogic;    
    displayLogic = NULL;
    delete displayLogicAdaptor;
    displayLogicAdaptor = NULL;
    delete lockScreenLogic;
    lockScreenLogic = NULL;
    delete shutdownLogic;
    shutdownLogic = NULL;
}
