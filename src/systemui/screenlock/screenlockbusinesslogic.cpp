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
#include <MApplicationExtensionArea>
#include <QTimer>
#include <QDBusInterface>
#include "screenlockextensioninterface.h"
#include "screenlockbusinesslogic.h"
#include "notifiernotificationsink.h"
#include "screenlockwindow.h"
#include "eventeater.h"
#include "closeeventeater.h"
#include "sysuid.h"

ScreenLockBusinessLogic::ScreenLockBusinessLogic(QObject* parent) :
    QObject(parent),
    screenLockWindow(NULL),
    eventEaterWindow(NULL),
    callbackInterface(NULL),
    shuttingDown(false)
{
#ifdef HAVE_QMSYSTEM
    connect(&displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(displayStateChanged(MeeGo::QmDisplayState::DisplayState)));
    connect(&systemState, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)), this, SLOT(systemStateChanged(MeeGo::QmSystemState::StateIndication)));
#endif

    // Create an extension area for the screen lock
    extensionArea = new MApplicationExtensionArea("com.meego.core.ScreenLockExtensionInterface/0.20");
    extensionArea->setStyleName("ScreenLockExtensionArea");
    extensionArea->setInProcessFilter(QRegExp("/sysuid-screenlock.desktop$"));
    extensionArea->setOutOfProcessFilter(QRegExp("$^"));
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), this, SLOT(registerExtension(MApplicationExtensionInterface*)));
    connect(extensionArea, SIGNAL(extensionRemoved(MApplicationExtensionInterface*)), this, SLOT(unregisterExtension(MApplicationExtensionInterface*)));
    extensionArea->init();
}

ScreenLockBusinessLogic::~ScreenLockBusinessLogic()
{
    delete screenLockWindow;
    delete eventEaterWindow;
}

int ScreenLockBusinessLogic::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool, bool)
{
    if (shuttingDown) {
        // Don't show the touch screen lock while shutting down
        return TkLockReplyOk;
    }

    // Create a D-Bus interface if one doesn't exist or the D-Bus callback details have changed
    if (callbackInterface == NULL || callbackInterface->service() != service || callbackInterface->path() != path || callbackInterface->interface() != interface) {
        delete callbackInterface;
        callbackInterface = new QDBusInterface(service, path, interface, QDBusConnection::systemBus(), this);
    }

    // Store the callback method name
    callbackMethod = method;

    // MCE needs a response ASAP, so the actions are executed with single shot timers
    switch (mode) {
    case TkLockModeEnable:
        // Create the lock screen already so that it's readily available
        QTimer::singleShot(0, this, SLOT(showScreenLock()));
        break;

    case TkLockModeOneInput:
        QTimer::singleShot(0, this, SLOT(showEventEater()));
        break;

    case TkLockEnableVisual:
        // Raise the lock screen window on top if it isn't already
        QTimer::singleShot(0, this, SLOT(showScreenLock()));
        break;

    case TkLockEnableLowPowerMode:
        // Enable low power mode and raise the lock screen window on top if it isn't already
        QTimer::singleShot(0, this, SLOT(showLowPowerMode()));
        break;

    case TkLockRealBlankMode:
        QTimer::singleShot(0, this, SLOT(setDisplayOffMode()));
        break;

    default:
        break;
    }

    return TkLockReplyOk;
}

int ScreenLockBusinessLogic::tklock_close(bool)
{
    QTimer::singleShot(0, this, SLOT(hideScreenLockAndEventEater()));

    return TkLockReplyOk;
}

void ScreenLockBusinessLogic::registerExtension(MApplicationExtensionInterface *extension)
{
    ScreenLockExtensionInterface *screenLockExtension = static_cast<ScreenLockExtensionInterface *>(extension);
    screenLockExtensions.append(screenLockExtension);
    screenLockExtension->setNotificationManagerInterface(Sysuid::instance()->notificationManagerInterface());
    connect(screenLockExtension->qObject(), SIGNAL(unlocked()), this, SLOT(unlockScreen()));
    connect(&Sysuid::instance()->notifierNotificationSink(), SIGNAL(notifierSinkActive(bool)), screenLockExtension->qObject(), SIGNAL(notifierSinkActive(bool)));
}

void ScreenLockBusinessLogic::unregisterExtension(MApplicationExtensionInterface *extension)
{
    ScreenLockExtensionInterface *screenLockExtension = static_cast<ScreenLockExtensionInterface *>(extension);

    if (screenLockExtension != NULL) {
        screenLockExtensions.removeAll(screenLockExtension);
    }
}

void ScreenLockBusinessLogic::reset()
{
    foreach (ScreenLockExtensionInterface *screenLockExtension, screenLockExtensions) {
        screenLockExtension->reset();
    }
}

void ScreenLockBusinessLogic::unlockScreen()
{
    if (screenLockWindow != NULL && screenLockWindow->isVisible()) {
        toggleScreenLockUI(false);
        toggleEventEater(false);

        if (callbackInterface != NULL && !callbackMethod.isEmpty()) {
            callbackInterface->call(QDBus::NoBlock, callbackMethod, TkLockUnlock);
        }
    }
}

void ScreenLockBusinessLogic::showScreenLock()
{
    // When the low power mode is switched off, it must be done before updating the window contents.
    // If the window does not yet exist, the window is created to be in the non-low power mode.
    if (screenLockWindow != NULL) {
        screenLockWindow->setLowPowerMode(false);
    }

    foreach (ScreenLockExtensionInterface *screenLockExtension, screenLockExtensions) {
        screenLockExtension->setMode(ScreenLockExtensionInterface::NormalMode);
    }
    toggleScreenLockUI(true);
    toggleEventEater(false);
}

void ScreenLockBusinessLogic::showLowPowerMode()
{
    ensureScreenLockWindowExists();

    // When the low power mode is switched on, it must be done after updating the window contents
    foreach (ScreenLockExtensionInterface *screenLockExtension, screenLockExtensions) {
        screenLockExtension->setMode(ScreenLockExtensionInterface::LowPowerMode);
    }

    toggleScreenLockUI(true);
    toggleEventEater(false);

    // When the low power mode is switched on, it must be done after updating the window contents
    screenLockWindow->setLowPowerMode(true);
}

void ScreenLockBusinessLogic::setDisplayOffMode()
{
    // When the low power mode is switched off, it must be done before updating the window contents.
    // If the window does not yet exist, the window is created to be in the non-low power mode.
    if (screenLockWindow != NULL) {
        screenLockWindow->setLowPowerMode(false);
    }

    foreach (ScreenLockExtensionInterface *screenLockExtension, screenLockExtensions) {
        screenLockExtension->setMode(ScreenLockExtensionInterface::DisplayOffMode);
    }
    toggleScreenLockUI(true);
    toggleEventEater(false);
}

void ScreenLockBusinessLogic::hideScreenLockAndEventEater()
{
    toggleScreenLockUI(false);
    toggleEventEater(false);
}

void ScreenLockBusinessLogic::showEventEater()
{
    toggleEventEater(true);
}

void ScreenLockBusinessLogic::hideEventEater()
{
    toggleEventEater(false);
}

void ScreenLockBusinessLogic::toggleScreenLockUI(bool toggle)
{

    if (toggle) {
        ensureScreenLockWindowExists();
        // Whenever we're showing the lock screen we need to reset its state
        reset();

        if (!screenLockWindow->isVisible()) {
            screenLockWindow->show();
        }
    } else {
        // Always switch the low power mode off before hiding the screen lock window
        foreach (ScreenLockExtensionInterface *screenLockExtension, screenLockExtensions) {
            screenLockExtension->setMode(ScreenLockExtensionInterface::NormalMode);
        }

        if (screenLockWindow != NULL && screenLockWindow->isVisible()) {
            screenLockWindow->setLowPowerMode(false);
            screenLockWindow->hide();
        }
    }
}

void ScreenLockBusinessLogic::toggleEventEater(bool toggle)
{
    if (toggle) {
        if (eventEaterWindow == NULL) {
            // Create the event eater window if it doesn't exist yet
            eventEaterWindow = new EventEater;
            eventEaterWindow->installEventFilter(new CloseEventEater(this));
            connect(eventEaterWindow, SIGNAL(inputEventReceived()), this, SLOT(hideEventEater()));
        }

        eventEaterWindow->show();
    } else {
        if (eventEaterWindow != NULL) {
            eventEaterWindow->hide();
        }
    }
}

void ScreenLockBusinessLogic::ensureScreenLockWindowExists()
{
    if (screenLockWindow == NULL) {
        screenLockWindow = new ScreenLockWindow(extensionArea);
        screenLockWindow->installEventFilter(new CloseEventEater(this));
    }
}

#ifdef HAVE_QMSYSTEM
void ScreenLockBusinessLogic::displayStateChanged(MeeGo::QmDisplayState::DisplayState state)
{
    // When the screen is unblanked and the screenlock is visible the lock screen needs to be reset
    if (state == MeeGo::QmDisplayState::On && screenLockWindow != NULL && screenLockWindow->isVisible()) {
        reset();
        screenLockWindow->setFocus();
    }
}

void ScreenLockBusinessLogic::systemStateChanged(MeeGo::QmSystemState::StateIndication what)
{
    switch (what) {
        case MeeGo::QmSystemState::Shutdown:
            // The lock screen should be disabled during shutdown
            hideScreenLockAndEventEater();
            shuttingDown = true;
            break;
        default:
            break;
    }
}
#endif
