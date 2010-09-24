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

#include <MLocale>
#include <MTheme>
#include <MLocale>

#include <QDBusConnection>
#include <QDebug>

#include "usbui.h"
#include "sysuid.h"
#include "sysuidrequest.h"
#include "batterybusinesslogic.h"
#include "lockscreenbusinesslogic.h"
#include "shutdownbusinesslogic.h"
#include "statusarearenderer.h"
#include "statusarearendereradaptor.h"
#include "statusindicatormenuadaptor.h"
#include "statusindicatormenuwindow.h"
#include "notificationmanager.h"
#include "mcompositornotificationsink.h"
#include "ngfnotificationsink.h"
#include "contextframeworkcontext.h"
#include "unlocknotificationsink.h"
#include "volumecontrolui.h"

#define DEBUG
#define WARNING
#include "debug.h"

#include <QX11Info>
#include <X11/Xutil.h>

#define TRANSLATION_CATALOG "systemui"

Sysuid* Sysuid::m_Sysuid = NULL;

Sysuid::Sysuid(QObject* parent) : QObject (parent)
{
    SYS_DEBUG ("Starting sysuidaemon");

    m_Sysuid = this;

    // Load translations of System-UI
    loadTranslations ();

    m_ShutdownLogic   = new ShutdownBusinessLogic (this);
    m_BatteryLogic    = new BatteryBusinessLogic (this);
    m_UsbUi           = new UsbUi (this);

    m_notificationManager        = new NotificationManager (3000);
    m_compositorNotificationSink = new MCompositorNotificationSink;
    m_ngfNotificationSink        = new NGFNotificationSink;
    m_unlockNotificationSink     = new UnlockNotificationSink;

    // D-Bus registration and stuff
    new ShutdownBusinessLogicAdaptor (this, m_ShutdownLogic);

    QDBusConnection bus = QDBusConnection::sessionBus ();
    if (!bus.registerService (dbusService ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus service";
        abort();
    }
    if (!bus.registerObject (dbusPath (), sysuid ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus object";
        abort();
    }

    // Create a status area renderer for rendering the shared status area pixmap
    m_statusAreaRenderer = new StatusAreaRenderer(this);
    new StatusAreaRendererAdaptor(m_statusAreaRenderer);
    bus.registerService("com.meego.core.MStatusBar");
    bus.registerObject("/statusbar", m_statusAreaRenderer);

    // Create a status indicator menu
    m_statusIndicatorMenuWindow = new StatusIndicatorMenuWindow;
    new StatusIndicatorMenuAdaptor(m_statusIndicatorMenuWindow);
    bus.registerService("com.meego.core.MStatusIndicatorMenu");
    bus.registerObject("/statusindicatormenu", m_statusIndicatorMenuWindow);
    connect(m_statusIndicatorMenuWindow, SIGNAL(visibilityChanged(bool)), m_statusAreaRenderer, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)));
    connect(m_statusIndicatorMenuWindow, SIGNAL(visibilityChanged(bool)), m_compositorNotificationSink, SLOT(setDisabled(bool)));

    // Connect the notification signals for the compositor notification sink
    connect (m_notificationManager, SIGNAL (notificationUpdated (const Notification &)),
             m_compositorNotificationSink, SLOT (addNotification (const Notification &)));
    connect (m_notificationManager, SIGNAL (notificationRemoved (uint)),
             m_compositorNotificationSink, SLOT (removeNotification (uint)));
    connect (m_compositorNotificationSink, SIGNAL (notificationRemovalRequested (uint)),
             m_notificationManager, SLOT (removeNotification (uint)));

    // Connect the notification signals for the feedback notification sink
    connect (m_notificationManager, SIGNAL (notificationUpdated (const Notification &)),
             m_ngfNotificationSink, SLOT (addNotification (const Notification &)));
    connect (m_notificationManager, SIGNAL (notificationRemoved (uint)),
              m_ngfNotificationSink, SLOT (removeNotification (uint)));

    // Connect the notification signals for the unlock-screen notification sink
    connect (m_notificationManager, SIGNAL (notificationUpdated (const Notification &)),
             m_unlockNotificationSink, SLOT (addNotification (const Notification &)));
    connect (m_notificationManager, SIGNAL (notificationRemoved (uint)),
             m_unlockNotificationSink, SLOT (removeNotification (uint)));

    // Subscribe to a context property for getting information about the video recording status
    ContextFrameworkContext context;
    useMode = QSharedPointer<ContextItem> (context.createContextItem ("Use.Mode"));
    connect (useMode.data (), SIGNAL (contentsChanged ()),
             this, SLOT (applyUseMode ()));
    applyUseMode ();

    // Restore persistent notifications after all the signal connections are made to the notification sinks
    m_notificationManager->restorePersistentData ();

    /*
     * The screen locking is implemented in this separate class, because it is
     * bound to the system bus (MCE wants to contact us on the system bus).
     */
    m_sysuidRequest = new SysUidRequest;

    // Connect the unlock-screen notification sink to LockScreenBusinessLogic
    if (m_sysuidRequest->getLockScreenLogic () != 0)
    {
        connect (m_sysuidRequest->getLockScreenLogic (),
                 SIGNAL (screenIsLocked (bool)),
                 m_unlockNotificationSink,
                 SLOT (setLockedState (bool)));
        connect (m_sysuidRequest->getLockScreenLogic (),
                 SIGNAL (screenIsLocked (bool)),
                 m_compositorNotificationSink,
                 SLOT (setDisabled (bool)));
        connect (m_sysuidRequest->getLockScreenLogic (),
                 SIGNAL (screenIsLocked (bool)),
                 m_UsbUi,
                 SLOT (setDisabled (bool)));
    }

    /*
     * Instantiate the volume-control UI
     */
    m_volumeBar = new VolumeControlUI;
}

Sysuid::~Sysuid ()
{
    m_Sysuid = 0;
    delete m_sysuidRequest;
    delete m_volumeBar;
    delete m_statusIndicatorMenuWindow;
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

void Sysuid::loadTranslations ()
{
    MLocale        locale;

    // Install real translations
    locale.installTrCatalog ("usb");
    locale.installTrCatalog ("reset");
    locale.installTrCatalog ("energy");
    locale.installTrCatalog ("shutdown");
    locale.installTrCatalog ("profiles");
    locale.installTrCatalog ("screenlock");
    locale.installTrCatalog ("status");
    // and the engineering english ones
    locale.installTrCatalog (TRANSLATION_CATALOG);

    MLocale::setDefault (locale);
}

NotificationManager &Sysuid::notificationManager ()
{
    return *m_notificationManager;
}

MCompositorNotificationSink& Sysuid::compositorNotificationSink ()
{
    return *m_compositorNotificationSink;
}

UnlockNotificationSink& Sysuid::unlockNotificationSink ()
{
    return *m_unlockNotificationSink;
}

void Sysuid::applyUseMode ()
{
    bool videoRecording =
        useMode->value ().toString () == "recording";

    m_compositorNotificationSink->setApplicationEventsEnabled (!videoRecording);
    m_ngfNotificationSink->setApplicationEventsEnabled (!videoRecording);
}

