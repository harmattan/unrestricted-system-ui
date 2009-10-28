#include <DuiLocale>
#include <DuiTheme>

#include <QDBusConnection>
#include <QDebug>

#include "sysuid.h"
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

Sysuid::Sysuid() :
        QObject()
{    
    qDebug() << "starting sysuidaemon";

    /* themes */
    DuiTheme::addPixmapDirectory(svgDir); // or ..(themeDir, true); ?
    DuiTheme::loadCSS(styleDir + "sysuid.css");
    DuiTheme::loadCSS(styleDir + "unlocksliderstyle.css");

    /* GConf interface */
    systemUIGConf = new SystemUIGConf(this);

    /* Pincode query variables */
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic(this);

    /* Display */
    displayLogic = new DisplayBusinessLogic(this);
    displayLogicAdaptor = new DisplayBusinessLogicAdaptor(dbusObject(), displayLogic);

    /* Network */
    networkLogic = new PhoneNetworkBusinessLogic(systemUIGConf, this);
    networkLogicAdaptor = new PhoneNetworkBusinessLogicAdaptor(dbusObject(), networkLogic);

    /* Event handler */
    eventHandler = new EventHandler(this);

    /* Shutdown */
    shutdownLogic = new ShutdownBusinessLogic(this);
    connect(eventHandler, SIGNAL(longPowerKeyPressOccured(bool)),
            shutdownLogic, SLOT(openDialog(bool)));
    connect(eventHandler, SIGNAL(powerKeyDownOccured()),
            shutdownLogic, SLOT(powerKeyDown()));
    connect(eventHandler, SIGNAL(powerKeyUpOccured()),
            shutdownLogic, SLOT(powerKeyUp()));

    /* Lockscreen */
    lockScreenLogic = new LockScreenBusinessLogic(this);
    connect(eventHandler, SIGNAL(shortPowerKeyPressOccured()), lockScreenLogic, SLOT(shortPowerKeyPressOccured()));    
    connect(displayLogic, SIGNAL(displayOff()), lockScreenLogic, SLOT(displayOff()));    
    connect(displayLogic, SIGNAL(displayOn()), lockScreenLogic, SLOT(displayOn()));
    connect(lockScreenLogic, SIGNAL(toggleDisplay(bool)), displayLogic, SLOT(toggleDisplay(bool)));

    /* Battery */
    batteryLogic = new BatteryBusinessLogic(systemUIGConf, lockScreenLogic, this);
    batteryLogicAdaptor = new BatteryBusinessLogicAdaptor(dbusObject(), batteryLogic);

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
    if(NULL != notifier())
        delete notifier();
    if(NULL != dbusObject())
        delete dbusObject();
}
