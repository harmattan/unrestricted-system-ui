#include <DuiLocale>
#include <DuiTheme>

#include <QDBusConnection>
#include <QDebug>
#include <QPointer>

#include "sysuid.h"

Sysuid::Sysuid() : QObject()
{    
    qDebug() << "starting sysuidaemon";

    /* GConf interface */
    systemUIGConf = new SystemUIGConf();

    /* Pincode query variables */
    DuiTheme::addPixmapDirectory("/usr/share/sysuid/themes/", true);
    DuiTheme::loadCSS("sysuid.css");
    DuiTheme::loadCSS("/usr/share/sysuid/themes/style/unlocksliderstyle.css");
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic();

    /* Battery */         
    batteryLogic = new BatteryBusinessLogic(systemUIGConf);
    batteryLogicAdaptor = new BatteryBusinessLogicAdaptor(dbusObject(), batteryLogic);

    /* Display */
    displayLogic = new DisplayBusinessLogic(systemUIGConf);
    displayLogicAdaptor = new DisplayBusinessLogicAdaptor(dbusObject(), displayLogic);

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
    if(!bus.registerService(dbusService())) {
        qDebug() << "failed to register dbus service";
        abort();
    }
    if(!bus.registerObject(dbusPath(), dbusObject())) {
        qDebug() << "failed to register dbus objects";
        abort();
    }
}

QPointer<QObject> Sysuid::dbusObject()
{
    static QPointer<QObject> o = NULL;
    if(o.isNull())
    {
        o = new QObject();
    }
    return o;
}

QPointer<Notifier> Sysuid::notifier()
{
    static QPointer<Notifier> n = NULL;
    if(n.isNull())
    {
        n = new Notifier();
    }
    return n;
}

QString Sysuid::dbusService()
{
    return QString("com.nokia.systemui");
}

QString Sysuid::dbusPath()
{
    return QString("/");
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
    if(NULL != notifier())
        delete notifier();
    if(NULL != dbusObject())
        delete dbusObject();
}
