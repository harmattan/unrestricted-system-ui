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
#include <MApplicationExtensionArea>
#include <QDBusConnection>

#include "usbui.h"
#include "sysuid.h"
#include "sysuidrequest.h"
#include "batterybusinesslogic.h"
#include "statusarearendereradaptor.h"
#include "screenlockbusinesslogic.h"
#include "shutdownbusinesslogic.h"
#include "statusarearenderer.h"
#include "statusindicatormenubusinesslogic.h"
#include "statusindicatormenuadaptor.h"
#include "notificationmanager.h"
#include "mcompositornotificationsink.h"
#include "ngfnotificationsink.h"
#include "contextframeworkcontext.h"
#include "notifiernotificationsink.h"
#include "closeeventeater.h"
#include <QX11Info>

Sysuid* Sysuid::instance_ = NULL;

static const QString DBUS_SERVICE = "com.nokia.systemui";
static const QString DBUS_PATH = "/";
static int NOTIFICATION_RELAY_INTERVAL = 5000;

Sysuid::Sysuid(QObject* parent) :
        QObject(parent)
{
    instance_ = this;

    // Load translations of System-UI
    loadTranslations();

    shutdownBusinessLogic = new ShutdownBusinessLogic(this);
    batteryBusinessLogic = new BatteryBusinessLogic(this);
    usbUi = new UsbUi(this);

    notificationManager = new NotificationManager(NOTIFICATION_RELAY_INTERVAL);
    mCompositorNotificationSink = new MCompositorNotificationSink;
    ngfNotificationSink = new NGFNotificationSink;
    notifierNotificationSink_ = new NotifierNotificationSink;

    // D-Bus registration and stuff
    new ShutdownBusinessLogicAdaptor(this, shutdownBusinessLogic);

    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService(DBUS_SERVICE)) {
        qCritical() << Q_FUNC_INFO << "failed to register dbus service";
        abort();
    }
    if (!bus.registerObject(DBUS_PATH, instance())) {
        qCritical() << Q_FUNC_INFO << "failed to register dbus object";
        abort();
    }

    // Create a status area renderer for rendering the shared status area pixmap
    statusAreaRenderer = new StatusAreaRenderer(this);
    new StatusAreaRendererAdaptor(statusAreaRenderer);
    bus.registerService("com.meego.core.MStatusBar");
    bus.registerObject("/statusbar", statusAreaRenderer);

    // Create a status indicator menu
    statusIndicatorMenuBusinessLogic = new StatusIndicatorMenuBusinessLogic(this);
    connect(statusIndicatorMenuBusinessLogic, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), statusAreaRenderer, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)));
    connect(statusIndicatorMenuBusinessLogic, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), this, SLOT(updateCompositorNotificationSinkEnabledStatus()));
    new StatusIndicatorMenuAdaptor(statusIndicatorMenuBusinessLogic);
    bus.registerService("com.meego.core.MStatusIndicatorMenu");
    bus.registerObject("/statusindicatormenu", statusIndicatorMenuBusinessLogic);

    // Connect the notification signals for the compositor notification sink
    connect (notificationManager, SIGNAL(notificationUpdated (const Notification &)),
            mCompositorNotificationSink, SLOT(addNotification (const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), mCompositorNotificationSink, SLOT(removeNotification(uint)));
    connect(mCompositorNotificationSink, SIGNAL(notificationRemovalRequested(uint)), notificationManager, SLOT(removeNotification(uint)));

    // Connect the notification signals for the feedback notification sink
    connect (notificationManager, SIGNAL(notificationUpdated (const Notification &)),
            ngfNotificationSink, SLOT(addNotification (const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), ngfNotificationSink, SLOT(removeNotification(uint)));

    // Connect the notification signals for the notifier notification sink
    connect(mCompositorNotificationSink, SIGNAL(notificationAdded(const Notification &)), notifierNotificationSink_, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), notifierNotificationSink_, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(notificationRestored(const Notification &)), notifierNotificationSink_, SLOT(addNotification(const Notification &)));
    connect(notifierNotificationSink_, SIGNAL(notifierSinkActive(bool)), notificationManager, SLOT(removeUnseenFlags(bool)));

    // Subscribe to a context property for getting information about the video recording status
    ContextFrameworkContext context;
    useMode = QSharedPointer<ContextItem> (context.createContextItem("com.nokia.policy.camera"));
    connect(useMode.data(), SIGNAL(contentsChanged()), this, SLOT(applyUseMode()));
    applyUseMode();

    // Restore persistent notifications after all the signal connections are made to the notification sinks
    notificationManager->restoreData();

    /*
     * The screen locking is implemented in this separate class, because it is
     * bound to the system bus (MCE wants to contact us on the system bus).
     */
    sysUidRequest = new SysUidRequest;
    if (sysUidRequest->screenLockBusinessLogic() != NULL) {
        // Unlock the touch screen lock when displaying the USB dialog
        connect(usbUi, SIGNAL(dialogShown()), sysUidRequest->screenLockBusinessLogic(), SLOT(unlockScreen()));
    }

    // Update the enabled status of compositor notification sink based on screen and device locks
#ifdef HAVE_QMSYSTEM
    connect (&qmLocks, SIGNAL(stateChanged (MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)), this, SLOT(updateCompositorNotificationSinkEnabledStatus()));
    connect(&displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(updateCompositorNotificationSinkEnabledStatus()));
#endif
    updateCompositorNotificationSinkEnabledStatus();

    // Create an extension area for the volume extension
    volumeExtensionArea = new MApplicationExtensionArea("com.meego.core.VolumeExtensionInterface/0.20");
    volumeExtensionArea->setInProcessFilter(QRegExp("/sysuid-volume.desktop$"));
    volumeExtensionArea->setOutOfProcessFilter(QRegExp("$^"));
    volumeExtensionArea->init();
}

Sysuid::~Sysuid()
{
    delete sysUidRequest;
    delete notifierNotificationSink_;
    delete ngfNotificationSink;
    delete mCompositorNotificationSink;
    delete notificationManager;
    delete volumeExtensionArea;
    instance_ = 0;
}

Sysuid* Sysuid::instance()
{
    return instance_;
}

void Sysuid::loadTranslations()
{
    MLocale locale;

    // Install real translations
    locale.installTrCatalog("usb");
    locale.installTrCatalog("reset");
    locale.installTrCatalog("energy");
    locale.installTrCatalog("shutdown");
    locale.installTrCatalog("profiles");
    locale.installTrCatalog("screenlock");
    locale.installTrCatalog("status");
    locale.installTrCatalog("notification");
    locale.installTrCatalog("connectivity");

    // and the engineering english ones
    locale.installTrCatalog("systemui");

    MLocale::setDefault(locale);
}

NotificationManagerInterface &Sysuid::notificationManagerInterface()
{
    return *notificationManager;
}

NotifierNotificationSink& Sysuid::notifierNotificationSink()
{
    return *notifierNotificationSink_;
}

void Sysuid::applyUseMode()
{
    bool videoRecording = useMode->value().toString() == "videorecording";

    mCompositorNotificationSink->setApplicationEventsEnabled(!videoRecording);
    ngfNotificationSink->setApplicationEventsEnabled(!videoRecording);
}

void Sysuid::updateCompositorNotificationSinkEnabledStatus()
{
    mCompositorNotificationSink->setDisabled(statusIndicatorMenuBusinessLogic->isStatusIndicatorMenuVisible()
#ifdef HAVE_QMSYSTEM
                                             || qmLocks.getState(MeeGo::QmLocks::Device) == MeeGo::QmLocks::Locked
                                             || qmLocks.getState(MeeGo::QmLocks::TouchAndKeyboard) == MeeGo::QmLocks::Locked
                                             || displayState.get() == MeeGo::QmDisplayState::Off
#endif
                                             );
}
