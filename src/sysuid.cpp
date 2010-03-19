/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <DuiApplicationWindow>
#include <DuiLocale>
#include <DuiTheme>
#include <DuiLocale>
#include <DuiGConfItem>

#include <QDBusConnection>
#include <QDebug>

#include "usbui.h"
#include "usbbusinesslogicadaptor.h"
#include "sysuid.h"
#include "sysuidrequest.h"
#include "systemuigconf.h"
#include "batterybusinesslogic.h"
#include "batterybusinesslogicadaptor.h"
#include "ledbusinesslogic.h"
#include "ledbusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"
#include "shutdownbusinesslogic.h"
#include "statusareawindow.h"
#include "notificationmanager.h"
#include "duicompositornotificationsink.h"
#include "duifeedbacknotificationsink.h"

//#define DEBUG
#define WARNING
#include "debug.h"

#define TRANSLATION_CATALOG "systemui"

namespace
{
const QString themeDir = "/usr/share/sysuid/themes/";
const QString styleDir = themeDir + "style/";
const QString svgDir = themeDir + "svg/";
}

Sysuid* Sysuid::m_Sysuid = NULL;

Sysuid::Sysuid () : QObject (),
        applicationWindow_(new DuiApplicationWindow)
{
    SYS_DEBUG ("Starting sysuidaemon");

    m_Sysuid = this;

    DuiTheme::addPixmapDirectory (svgDir); // or ..(themeDir, true); ?
    DuiTheme::loadCSS (styleDir + "sysuid.css");
    DuiTheme::loadCSS (styleDir + "unlocksliderstyle.css");

    // Load translation of System-UI
    retranslate ();

    applicationWindow_->setWindowOpacity(0.0);
    applicationWindow_->setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    m_SystemUIGConf   = new SystemUIGConf (this);
    m_ShutdownLogic   = new ShutdownBusinessLogic (this);
    m_BatteryLogic    = new BatteryBusinessLogic (m_SystemUIGConf, this);
    m_LedLogic        = new LedBusinessLogic (m_SystemUIGConf, this);
    m_UsbUi           = new UsbUi (this);

    notificationManager_ = new NotificationManager(3000);
    compositorNotificationSink_ = new DuiCompositorNotificationSink;
    feedbackNotificationSink_ = new DuiFeedbackNotificationSink;    

    // D-Bus registration and stuff
    new BatteryBusinessLogicAdaptor (this, m_BatteryLogic);
    new LedBusinessLogicAdaptor (this, m_LedLogic);
    new UsbBusinessLogicAdaptor (this, m_UsbUi->getLogic ());

    QDBusConnection bus = QDBusConnection::sessionBus ();
    if (!bus.registerService (dbusService ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus service";
        abort();
    }
    if (!bus.registerObject (dbusPath (), sysuid ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus object";
        abort();
    }

    // Show status area window when sysui daemon starts
    statusAreaWindow_ = new StatusAreaWindow;
    statusAreaWindow_->show();

    // Connect the notification signals for the compositor notification sink
    connect(notificationManager_, SIGNAL(notificationUpdated(const Notification &)), compositorNotificationSink_, SLOT(addNotification(const Notification &)));
    connect(notificationManager_, SIGNAL(notificationRemoved(uint)), compositorNotificationSink_, SLOT(removeNotification(uint)));
    connect(compositorNotificationSink_, SIGNAL(notificationRemovalRequested(uint)), notificationManager_, SLOT(removeNotification(uint)));
    connect(notificationManager_, SIGNAL(notificationRestored(const Notification &)), compositorNotificationSink_, SIGNAL(notificationAdded(Notification)));

    // Connect the notification signals for the feedback notification sink
    connect(notificationManager_, SIGNAL(notificationUpdated(const Notification &)), feedbackNotificationSink_, SLOT(addNotification(const Notification &)));
    connect(notificationManager_, SIGNAL(notificationRemoved(uint)), feedbackNotificationSink_, SLOT(removeNotification(uint)));
    connect(statusAreaWindow_, SIGNAL(orientationChangeFinished(const Dui::Orientation &)), this, SIGNAL(orientationChangeFinished(const Dui::Orientation &)));
    // Restore persistent notifications after all the signal connections are made to the notification sinks
    notificationManager_->restorePersistentData();

    /*
     * The screen locking is implemented in this separate class, because it is
     * bound to the system bus (MCE wants to contact us on the system bus).
     */
    new SysUidRequest;
}

Sysuid::~Sysuid ()
{
    m_Sysuid = NULL;
    delete statusAreaWindow_;
    delete applicationWindow_;
}

Sysuid* Sysuid::sysuid ()
{
    return m_Sysuid;
}

QString Sysuid::dbusService ()
{
    return QString ("com.nokia.systemui");
}

QString Sysuid::dbusPath ()
{
    return QString ("/");
}

/*!
 * Please note that in the libdui 0.19.4 manipulating theh DuiLocale in this
 * function might cause an endless recursion. I added a protection for brake the
 * recursion.
 *
 * FIXME: Once DuiLocale is working as it should be this function could be
 * eliminated.
 */
void Sysuid::retranslate ()
{
    static bool      running = false;

    SYS_DEBUG ("*** running = %s", running ? "true" : "false");
    if (running)
        return;
    running = true;

    DuiGConfItem     languageItem ("/Dui/i18n/Language");
    QString          lang = languageItem.value ().toString ();
    // Create a new duilocale instance with current language
    DuiLocale        locale (lang);

    SYS_DEBUG (" lang = %s", SYS_STR (lang));

    // Install engineering english
    locale.installTrCatalog (TRANSLATION_CATALOG ".qm");
    // Install real translation
    locale.installTrCatalog (TRANSLATION_CATALOG);

    DuiLocale::setDefault (locale);

    running = false;
}

NotificationManager &Sysuid::notificationManager()
{
    return *notificationManager_;
}

DuiCompositorNotificationSink& Sysuid::compositorNotificationSink()
{
    return *compositorNotificationSink_;
}

Dui::Orientation Sysuid::orientation() const
{
    return statusAreaWindow_->orientation();
}

Dui::OrientationAngle Sysuid::orientationAngle() const
{
    return statusAreaWindow_->orientationAngle();
}

DuiApplicationWindow &Sysuid::applicationWindow()
{
    return *applicationWindow_;
}
