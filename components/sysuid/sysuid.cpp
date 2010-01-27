/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <DuiLocale>
#include <DuiTheme>
#include <DuiLocale>
#include <DuiGConfItem>

#include <QDBusConnection>
#include <QDebug>

#include "sysuid.h"
#include "systemuigconf.h"
#include "batterybusinesslogic.h"
#include "batterybusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"
#include "shutdownbusinesslogic.h"

#define DEBUG
#include "../debug.h"

#define TRANSLATION_CATALOG "duicontrolpanel-systemui"

namespace
{
const QString themeDir = "/usr/share/sysuid/themes/";
const QString styleDir = themeDir + "style/";
const QString svgDir = themeDir + "svg/";
}

Sysuid* Sysuid::m_Sysuid = NULL;

Sysuid::Sysuid () : QObject ()
{
    qDebug () << "starting sysuidaemon";

    m_Sysuid = this;

    DuiTheme::addPixmapDirectory (svgDir); // or ..(themeDir, true); ?
    DuiTheme::loadCSS (styleDir + "sysuid.css");
    DuiTheme::loadCSS (styleDir + "unlocksliderstyle.css");

    // Load translation of System-UI
    retranslate ();

    m_SystemUIGConf   = new SystemUIGConf (this);
    m_ShutdownLogic   = new ShutdownBusinessLogic (this);
    m_LockScreenLogic = new LockScreenBusinessLogic (this);
    m_BatteryLogic    = new BatteryBusinessLogic (m_SystemUIGConf, this);

    // D-Bus registration and stuff
    new BatteryBusinessLogicAdaptor (this, m_BatteryLogic);
    new LockScreenBusinessLogicAdaptor (this, m_LockScreenLogic);

#if 0
    // This is what used in MCE.
    QDBusConnection bus = QDBusConnection::systemBus ();
#else
    // This is what others want.
    QDBusConnection bus = QDBusConnection::sessionBus ();
#endif
    if (!bus.registerService (dbusService ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus service";
        abort();
    }
    if (!bus.registerObject (dbusPath (), sysuid ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus object";
        abort();
    }
}

Sysuid::~Sysuid ()
{
    m_Sysuid = NULL;
}

Sysuid* Sysuid::sysuid ()
{
    return m_Sysuid;
}

QString Sysuid::dbusService ()
{
#if 0
    // This is what used in MCE.
    return QString ("com.nokia.system_ui");
#else
    // This is what others want.
    return QString ("com.nokia.systemui");
#endif
}

QString Sysuid::dbusPath ()
{
#if 0
    // This is what used in MCE.
    return QString ("/com/nokia/system_ui/request");
#else
    // This is what others want.
    return QString ("/");
#endif
}

void Sysuid::retranslate ()
{
    DuiGConfItem     languageItem ("/Dui/i18n/Language");
    QString          lang = languageItem.value ().toString ();
    // Create a new duilocale instance with current language
    DuiLocale        locale (lang);

    qDebug () << Q_FUNC_INFO << "with lang" << lang;

    // Install engineering english
    locale.installTrCatalog (TRANSLATION_CATALOG ".qm");
    // Install real translation
    locale.installTrCatalog (TRANSLATION_CATALOG);

    DuiLocale::setDefault (locale);
}

