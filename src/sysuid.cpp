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
    batteryLogicAdaptor = new BatteryBusinessLogicAdaptor(this, batteryLogic);

    /* Display */
    displayLogic = new DisplayBusinessLogic(systemUIGConf);
    displayLogicAdaptor = new DisplayBusinessLogicAdaptor(this, displayLogic);

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

    // D-Bus registration and stuff.
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService(QString("com.nokia.systemui"))) {
        qDebug() << "failed to register dbus service";
        abort();
    }
    if(!bus.registerObject(QString("/"), this)) {
        qDebug() << "failed to register dbus objects";
        abort();
    }
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
