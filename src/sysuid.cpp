#include <DuiLocale>
#include <DuiTheme>

#include <QDBusConnection>
#include <QDebug>

#include "sysuid.h"
#include "profilebusinesslogic.h"
#include "profilebusinesslogicadaptor.h"
#include "systemuigconf.h"
#include "pincodequerybusinesslogic.h"
#include "batterybusinesslogic.h"
#include "batterybusinesslogicadaptor.h"
#include "displaybusinesslogic.h"
#include "displaybusinesslogicadaptor.h"
#include "phonenetworkbusinesslogic.h"
#include "phonenetworkbusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"
#include "shutdownbusinesslogic.h"
#include "eventhandler.h"
#include "notifier.h"

namespace {
    const QString themeDir = "/usr/share/sysuid/themes/";
    const QString styleDir = themeDir + "style/";
    const QString svgDir = themeDir + "svg/";
}

Sysuid::Sysuid() : QObject()
{    
    qDebug() << "starting sysuidaemon";

    /* themes */
    DuiTheme::addPixmapDirectory(svgDir); // or ..(themeDir, true); ?
    DuiTheme::loadCSS(styleDir + "sysuid.css");
    DuiTheme::loadCSS(styleDir + "unlocksliderstyle.css");

    /* GConf interface */
    systemUIGConf = new SystemUIGConf();

    /* Pincode query variables */
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic();

    /* Battery */         
    batteryLogic = new BatteryBusinessLogic(systemUIGConf);
    batteryLogicAdaptor = new BatteryBusinessLogicAdaptor(dbusObject(), batteryLogic);

    /* Display */
    displayLogic = new DisplayBusinessLogic();
    displayLogicAdaptor = new DisplayBusinessLogicAdaptor(dbusObject(), displayLogic);

    /* Network */
    networkLogic = new PhoneNetworkBusinessLogic(systemUIGConf);
    networkLogicAdaptor = new PhoneNetworkBusinessLogicAdaptor(dbusObject(), networkLogic);

    /* Profile */
    profileLogic = new ProfileBusinessLogic();
    profileLogicAdaptor = new ProfileBusinessLogicAdaptor(dbusObject(), profileLogic);

    /* Event handler */
    eventHandler = new EventHandler();

    /* Shutdown */
    shutdownLogic = new ShutdownBusinessLogic();
    connect(eventHandler, SIGNAL(longPowerKeyPressOccured(bool)),
            shutdownLogic, SLOT(openDialog(bool)));
    connect(eventHandler, SIGNAL(powerKeyDownOccured()),
            shutdownLogic, SLOT(powerKeyDown()));
    connect(eventHandler, SIGNAL(powerKeyUpOccured()),
            shutdownLogic, SLOT(powerKeyUp()));

    /* Lockscreen */
    lockScreenLogic = new LockScreenBusinessLogic();    
    connect(lockScreenLogic, SIGNAL(lockScreenOff()), batteryLogic, SLOT(initBattery()));
    connect(batteryLogic, SIGNAL(batteryCharging()), lockScreenLogic, SLOT(sleepModeOff()));
    connect(eventHandler, SIGNAL(shortPowerKeyPressOccured()), lockScreenLogic, SLOT(shortPowerKeyPressOccured()));
    connect(shutdownLogic, SIGNAL(dialogOpen(bool)), lockScreenLogic, SLOT(disable(bool)));

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
    delete profileLogic;
    profileLogic = NULL;
    delete profileLogicAdaptor;
    profileLogicAdaptor = NULL;
    delete lockScreenLogic;
    lockScreenLogic = NULL;
    delete shutdownLogic;
    shutdownLogic = NULL;
    if(NULL != notifier())
        delete notifier();
    if(NULL != dbusObject())
        delete dbusObject();
}
