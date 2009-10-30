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
//#include "shutdownbusinesslogic.h"
#include "eventhandler.h"
#include "notifier.h"
#include "notificationtype.h"

namespace
{
const QString themeDir = "/usr/share/sysuid/themes/";
const QString styleDir = themeDir + "style/";
const QString svgDir = themeDir + "svg/";
}

Sysuid* Sysuid::_sysuid = NULL;

Sysuid::Sysuid() :
        QObject(),
        notifier(NULL)
{
    qDebug() << "starting sysuidaemon";

    _sysuid = this;    

    /* themes */
    DuiTheme::addPixmapDirectory(svgDir); // or ..(themeDir, true); ?
    DuiTheme::loadCSS(styleDir + "sysuid.css");
    DuiTheme::loadCSS(styleDir + "unlocksliderstyle.css");

    /* Notifier */
    notifier = new Notifier(this);

    /* GConf interface */
    systemUIGConf = new SystemUIGConf(this);

    /* Pincode query variables */    
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic(this);    
    connect(notifier->responseObject(), SIGNAL(pinQueryCanceled()), pinCodeQueryLogic, SLOT(cancelQuery()));    
    connect(notifier->responseObject(), SIGNAL(doSimLockRetry()), pinCodeQueryLogic, SLOT(resendSimLockCode()));
    connect(pinCodeQueryLogic, SIGNAL(showNotification(QString, NotificationType::Type)), notifier, SLOT(showNotification(QString, NotificationType::Type)));
    connect(pinCodeQueryLogic, SIGNAL(showConfirmation(QString, QString)), notifier, SLOT(showConfirmation(QString, QString)));

    /* Display */
    displayLogic = new DisplayBusinessLogic(this);

    /* Network */
    networkLogic = new PhoneNetworkBusinessLogic(systemUIGConf, this);

    /* Event handler */
    eventHandler = new EventHandler(this);

    /* Shutdown */
    /*
        shutdownLogic = new ShutdownBusinessLogic(this);

        connect(eventHandler, SIGNAL(longPowerKeyPressOccured(bool)),
                shutdownLogic, SLOT(openDialog(bool)));
        connect(eventHandler, SIGNAL(powerKeyDownOccured()),
                shutdownLogic, SLOT(powerKeyDown()));
        connect(eventHandler, SIGNAL(powerKeyUpOccured()),
                shutdownLogic, SLOT(powerKeyUp()));
    */
    /* Lockscreen */
    lockScreenLogic = new LockScreenBusinessLogic(this);
    connect(eventHandler, SIGNAL(shortPowerKeyPressOccured()), lockScreenLogic, SLOT(shortPowerKeyPressOccured()));

    /* Battery */
    batteryLogic = new BatteryBusinessLogic(systemUIGConf, this);
    connect(batteryLogic, SIGNAL(showNotification(QString)), notifier, SLOT(showNotification(QString)));

    // D-Bus registration and stuff.
    new DisplayBusinessLogicAdaptor(this, displayLogic);
    new PhoneNetworkBusinessLogicAdaptor(this, networkLogic);
    new BatteryBusinessLogicAdaptor(this, batteryLogic);

    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService(dbusService())) {
        qDebug() << "failed to register dbus service";
        abort();
    }
    if (!bus.registerObject(dbusPath(), sysuid())) {
        qDebug() << "failed to register dbus objects";
        abort();
    }
}

Sysuid::~Sysuid()
{
    _sysuid = NULL;
}

Sysuid* Sysuid::sysuid()
{
    return _sysuid;
}

QString Sysuid::dbusService()
{
    return QString("com.nokia.systemui");
}

QString Sysuid::dbusPath()
{
    return QString("/");
}


