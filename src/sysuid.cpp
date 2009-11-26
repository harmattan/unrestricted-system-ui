#include <DuiLocale>
#include <DuiTheme>

#include <QDBusConnection>
#include <QDebug>

#include "sysuid.h"
#include "systemuigconf.h"
#include "pincodequerybusinesslogic.h"
#include "batterybusinesslogic.h"
#include "batterybusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"
#include "notifier.h"
#include "notificationtype.h"
#include "shutdownbusinesslogic.h"
#include "cellularui.h"

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

    /* Shutdown */
    shutdownLogic = new ShutdownBusinessLogic(this);
    connect(shutdownLogic, SIGNAL(showNotification(QString, NotificationType::Type)),
            notifier, SLOT(showNotification(QString, NotificationType::Type)));

    /* Lockscreen */
    lockScreenLogic = new LockScreenBusinessLogic(this);

    /* Battery */
    batteryLogic = new BatteryBusinessLogic(systemUIGConf, this);
    connect(batteryLogic, SIGNAL(showNotification(QString)), notifier, SLOT(showNotification(QString)));

    /* Cellular UI */
    cellUI = new CellularUI(this);
    connect(cellUI, SIGNAL(showNotification(QString, NotificationType::Type)),
            notifier, SLOT(showNotification(QString, NotificationType::Type)));
    connect(cellUI, SIGNAL(showConfirmation(QString, QString)),
            notifier, SLOT(showConfirmation(QString, QString)));

    // D-Bus registration and stuff.
    new BatteryBusinessLogicAdaptor(this, batteryLogic);
    new LockScreenBusinessLogicAdaptor(this, lockScreenLogic);

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


