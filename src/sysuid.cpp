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
#include "contextframeworkcontext.h"

#define DEBUG
#define WARNING
#include "debug.h"

#define TRANSLATION_CATALOG "systemui"

namespace
{
const QString themeDir = "/usr/share/themes/base/dui/sysuid/";
const QString styleDir = themeDir + "style/";
const QString svgDir = themeDir + "svg/";
}

Sysuid* Sysuid::m_Sysuid = NULL;

Sysuid::Sysuid () : QObject (),
        m_applicationWindow(new DuiApplicationWindow)
{
    SYS_DEBUG ("Starting sysuidaemon");

    m_Sysuid = this;

    DuiTheme::addPixmapDirectory (svgDir); // or ..(themeDir, true); ?
    DuiTheme::loadCSS (styleDir + "sysuid.css");
    DuiTheme::loadCSS (styleDir + "unlocksliderstyle.css");

    // Load translation of System-UI
    retranslate ();

    m_applicationWindow->setWindowOpacity(0.0);
    m_applicationWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    m_SystemUIGConf   = new SystemUIGConf (this);
    m_ShutdownLogic   = new ShutdownBusinessLogic (this);
    m_BatteryLogic    = new BatteryBusinessLogic (m_SystemUIGConf, this);
    m_LedLogic        = new LedBusinessLogic (m_SystemUIGConf, this);
    m_UsbUi           = new UsbUi (this);

    m_notificationManager = new NotificationManager(3000);
    m_compositorNotificationSink = new DuiCompositorNotificationSink;
    m_feedbackNotificationSink = new DuiFeedbackNotificationSink;

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
    m_statusAreaWindow = new StatusAreaWindow;
    m_statusAreaWindow->show();
    connect(m_statusAreaWindow, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), m_compositorNotificationSink, SLOT(setDisabled(bool)));
    // Connect the notification signals for the compositor notification sink
    connect(m_notificationManager, SIGNAL(notificationUpdated(const Notification &)), m_compositorNotificationSink, SLOT(addNotification(const Notification &)));
    connect(m_notificationManager, SIGNAL(notificationRemoved(uint)), m_compositorNotificationSink, SLOT(removeNotification(uint)));
    connect(m_compositorNotificationSink, SIGNAL(notificationRemovalRequested(uint)), m_notificationManager, SLOT(removeNotification(uint)));
    connect(m_notificationManager, SIGNAL(notificationRestored(const Notification &)), m_compositorNotificationSink, SIGNAL(notificationAdded(Notification)));

    // Connect the notification signals for the feedback notification sink
    connect(m_notificationManager, SIGNAL(notificationUpdated(const Notification &)), m_feedbackNotificationSink, SLOT(addNotification(const Notification &)));
    connect(m_notificationManager, SIGNAL(notificationRemoved(uint)), m_feedbackNotificationSink, SLOT(removeNotification(uint)));
    connect(m_statusAreaWindow, SIGNAL(orientationChangeFinished(const Dui::Orientation &)), this, SIGNAL(orientationChangeFinished(const Dui::Orientation &)));

    // Subscribe to a context property for getting information about the video recording status
    ContextFrameworkContext context;
    useMode = QSharedPointer<ContextItem>(context.createContextItem("Use.Mode"));
    connect(useMode.data(), SIGNAL(contentsChanged()), this, SLOT(applyUseMode()));
    applyUseMode();

    // Restore persistent notifications after all the signal connections are made to the notification sinks
    m_notificationManager->restorePersistentData();

    /*
     * The screen locking is implemented in this separate class, because it is
     * bound to the system bus (MCE wants to contact us on the system bus).
     */
    new SysUidRequest;
}

Sysuid::~Sysuid ()
{
    m_Sysuid = NULL;
    delete m_statusAreaWindow;
    delete m_applicationWindow;
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

    DuiLocale        locale;

    SYS_DEBUG (" lang = %s", SYS_STR (locale.language ()));

    // Install engineering english
    locale.installTrCatalog (TRANSLATION_CATALOG ".qm");
    // Install real translation
    locale.installTrCatalog (TRANSLATION_CATALOG);

    DuiLocale::setDefault (locale);

    running = false;
}

NotificationManager &Sysuid::notificationManager()
{
    return *m_notificationManager;
}

DuiCompositorNotificationSink& Sysuid::compositorNotificationSink()
{
    return *m_compositorNotificationSink;
}

Dui::Orientation Sysuid::orientation() const
{
    return m_statusAreaWindow->orientation();
}

Dui::OrientationAngle Sysuid::orientationAngle() const
{
    return m_statusAreaWindow->orientationAngle();
}

DuiApplicationWindow &Sysuid::applicationWindow()
{
    return *m_applicationWindow;
}

void Sysuid::applyUseMode()
{
    bool videoRecording = useMode->value().toString() == "recording";

    m_compositorNotificationSink->setApplicationEventsEnabled(!videoRecording);
    m_feedbackNotificationSink->setApplicationEventsEnabled(!videoRecording);
}
