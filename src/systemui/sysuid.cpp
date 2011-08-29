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
#include "screenlockbusinesslogic.h"
#include "screenlockbusinesslogicadaptor.h"
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
#include "notificationstatusindicatorsink.h"
#include "closeeventeater.h"
#include "diskspacenotifier.h"
#include <QX11Info>

Sysuid* Sysuid::instance_ = NULL;

static const char *SYSTEMUI_DBUS_SERVICE = "com.nokia.systemui";
static const char *SYSTEMUI_DBUS_PATH = "/";
static const char *SCREENLOCK_DBUS_SERVICE = "com.nokia.system_ui";
static const char *SCREENLOCK_DBUS_PATH = "/com/nokia/system_ui/request";
static int NOTIFICATION_RELAY_INTERVAL = 5000;

Sysuid::Sysuid(QObject* parent) : QObject(parent)
{
    instance_ = this;

    // Load translations of System-UI
    loadTranslations();

    // D-Bus registration
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService(SYSTEMUI_DBUS_SERVICE)) {
        qCritical("Unable to register system-ui D-Bus service %s: %s", SYSTEMUI_DBUS_SERVICE, bus.lastError().message().toUtf8().constData());
        abort();
    }
    if (!bus.registerObject(SYSTEMUI_DBUS_PATH, instance())) {
        qCritical("Unable to register system-ui object at path %s: %s", SYSTEMUI_DBUS_PATH, bus.lastError().message().toUtf8().constData());
        abort();
    }

    // Initialize notification system
    notificationManager = new NotificationManager(NOTIFICATION_RELAY_INTERVAL);
    mCompositorNotificationSink = new MCompositorNotificationSink;
    ngfNotificationSink = new NGFNotificationSink;
    notificationStatusIndicatorSink_ = new NotificationStatusIndicatorSink;

    // Connect the notification signals for the compositor notification sink
    connect(notificationManager, SIGNAL(notificationUpdated(const Notification &)), mCompositorNotificationSink, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), mCompositorNotificationSink, SLOT(removeNotification(uint)));
    connect(mCompositorNotificationSink, SIGNAL(notificationRemovalRequested(uint)), notificationManager, SLOT(removeNotification(uint)));

    // Connect the notification signals for the feedback notification sink
    connect(notificationManager, SIGNAL(notificationUpdated(const Notification &)), ngfNotificationSink, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), ngfNotificationSink, SLOT(removeNotification(uint)));

    // Connect the notification signals for the notification status indicator sink
    connect(notificationManager, SIGNAL(notificationUpdated(const Notification &)), notificationStatusIndicatorSink_, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), notificationStatusIndicatorSink_, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(notificationRestored(const Notification &)), notificationStatusIndicatorSink_, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(groupUpdated(uint, const NotificationParameters &)), notificationStatusIndicatorSink_, SLOT(addGroup(uint, const NotificationParameters &)));

    // Subscribe to a context property for getting information about the video recording status
    ContextFrameworkContext context;
    useMode = QSharedPointer<ContextItem>(context.createContextItem("/com/nokia/policy/camera"));
    useMode.data()->subscribe();
    connect(useMode.data(), SIGNAL(contentsChanged()), this, SLOT(applyUseMode()));
    applyUseMode();

    // Create shut down UI
    shutdownBusinessLogic = new ShutdownBusinessLogic(this);
    new ShutdownBusinessLogicAdaptor(this, shutdownBusinessLogic);

    // Create a status area renderer for rendering the shared status area pixmap
    statusAreaRenderer = new StatusAreaRenderer(this);
    new StatusAreaRendererAdaptor(statusAreaRenderer);
    bus.registerService("com.meego.core.MStatusBar");
    bus.registerObject("/statusbar", statusAreaRenderer);

    // Create a status indicator menu
    statusIndicatorMenuBusinessLogic = new StatusIndicatorMenuBusinessLogic(this);
    connect(statusIndicatorMenuBusinessLogic, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), this, SLOT(updateCompositorNotificationSinkEnabledStatus()));
    new StatusIndicatorMenuAdaptor(statusIndicatorMenuBusinessLogic);
    bus.registerService("com.meego.core.MStatusIndicatorMenu");
    bus.registerObject("/statusindicatormenu", statusIndicatorMenuBusinessLogic);

    // Create screen lock business logic
    screenLockBusinessLogic = new ScreenLockBusinessLogic(this);
    new ScreenLockBusinessLogicAdaptor(screenLockBusinessLogic);
    connect(screenLockBusinessLogic, SIGNAL(screenIsLocked(bool)), this, SLOT(updateCompositorNotificationSinkEnabledStatus()));
    connect(screenLockBusinessLogic, SIGNAL(screenIsLocked(bool)), mCompositorNotificationSink, SLOT(setTouchScreenLockActive(bool)));

    // MCE expects the service to be registered on the system bus
    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService(SCREENLOCK_DBUS_SERVICE)) {
        qWarning("Unable to register screen lock D-Bus service %s: %s", SCREENLOCK_DBUS_SERVICE, systemBus.lastError().message().toUtf8().constData());
    }
    if (!systemBus.registerObject(SCREENLOCK_DBUS_PATH, screenLockBusinessLogic)) {
        qWarning("Unable to register screen lock object at path %s: %s", SCREENLOCK_DBUS_PATH, systemBus.lastError().message().toUtf8().constData());
    }

    // Update the enabled status of compositor notification sink based on screen and device locks
#ifdef HAVE_QMSYSTEM
    connect(&qmLocks, SIGNAL(stateChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)), this, SLOT(updateCompositorNotificationSinkEnabledStatus()));
#endif
    updateCompositorNotificationSinkEnabledStatus();

    // Create an extension area for the volume extension
    volumeExtensionArea = new MApplicationExtensionArea("com.meego.core.VolumeExtensionInterface/0.20");
    volumeExtensionArea->setInProcessFilter(QRegExp("/sysuid-volume.desktop$"));
    volumeExtensionArea->setOutOfProcessFilter(QRegExp("$^"));
    volumeExtensionArea->init();

    // Initialize notifications store after all the signal connections are made to the notification sinks but before any components that may send/remove notifications
    notificationManager->initializeStore();

    // Create components that may create or remove notifications
    batteryBusinessLogic = new BatteryBusinessLogic(this);
    connect(screenLockBusinessLogic, SIGNAL(screenIsLocked(bool)), batteryBusinessLogic, SLOT(setTouchScreenLockActive(bool)));

    usbUi = new UsbUi(this);

    new DiskSpaceNotifier(this);

    // Unlock the touch screen lock when displaying the USB dialog
    connect(usbUi, SIGNAL(dialogShown()), screenLockBusinessLogic, SLOT(unlockScreen()));
}

Sysuid::~Sysuid()
{
    delete notificationStatusIndicatorSink_;
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
    locale.installTrCatalog("volume");
    locale.installTrCatalog("memory-usage");
    locale.installTrCatalog("cellular-adaptation");

    // and the engineering english ones
    locale.installTrCatalog("systemui");

    MLocale::setDefault(locale);
}

NotificationManagerInterface &Sysuid::notificationManagerInterface()
{
    return *notificationManager;
}

NotificationStatusIndicatorSink& Sysuid::notificationStatusIndicatorSink()
{
    return *notificationStatusIndicatorSink_;
}

void Sysuid::applyUseMode()
{
    bool videoRecording = useMode->value().toString() == "videorecording";

    mCompositorNotificationSink->setApplicationEventsEnabled(!videoRecording);
    ngfNotificationSink->setApplicationEventsEnabled(!videoRecording);
}

void Sysuid::updateCompositorNotificationSinkEnabledStatus()
{
    mCompositorNotificationSink->setApplicationEventsDisabled(statusIndicatorMenuBusinessLogic->isStatusIndicatorMenuVisible() || screenLockBusinessLogic->isScreenLocked()
#ifdef HAVE_QMSYSTEM
                                             || qmLocks.getState(MeeGo::QmLocks::Device) == MeeGo::QmLocks::Locked
#endif
                                             );
}
