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
#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>
#include "mnamespace.h"
#include <QSharedPointer>
#include "applicationcontext.h"

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

class UsbUi;
class BatteryBusinessLogic;
class ShutdownBusinessLogic;
class StatusAreaRenderer;
class StatusIndicatorMenuBusinessLogic;
class NotificationManager;
class NotificationManagerInterface;
class MCompositorNotificationSink;
class NGFNotificationSink;
class UnlockNotificationSink;
class NotifierNotificationSink;
class ScreenLockBusinessLogic;
class VolumeBarLogic;
class MApplicationExtensionArea;

class Sysuid : public QObject
{
    Q_OBJECT

public:
    Sysuid(QObject* parent);
    virtual ~Sysuid();
    static Sysuid* instance();

    /*!
     * Returns a reference to the notification manager interface.
     *
     * \return a reference to the notification manager interface
     */
    NotificationManagerInterface &notificationManagerInterface();

    /*!
     * Returns a reference to the notifier notification sink.
     *
     * \return a reference to the notifier notification sink
     */
    NotifierNotificationSink& notifierNotificationSink();

signals:
    /*!
      * Inform about orientation changes
      */
    void orientationChangeFinished(const M::Orientation &);

private slots:
    /*!
     * Enables or disables various sinks according to current use mode
     */
    void applyUseMode();

    /*!
     * Enables or disables the compositor sink based on current state
     */
    void updateCompositorNotificationSinkEnabledStatus();

private:
    void loadTranslations();

private:
    BatteryBusinessLogic *batteryBusinessLogic;
    ShutdownBusinessLogic *shutdownBusinessLogic;
    UsbUi *usbUi;
    static Sysuid *instance_;

    //! Status area
    StatusAreaRenderer *statusAreaRenderer;

    //! Status indicator menu
    StatusIndicatorMenuBusinessLogic *statusIndicatorMenuBusinessLogic;

    //! Notification manager interface
    NotificationManager *notificationManager;

    //! Notification sink for visualizing the notification outside home
    MCompositorNotificationSink *mCompositorNotificationSink;

    //! Notification sink for presenting the notification as a feedback
    NGFNotificationSink *ngfNotificationSink;

    //! Notification sink for showing unread notifications
    NotifierNotificationSink *notifierNotificationSink_;

    //! The lock screen business logic
    ScreenLockBusinessLogic *screenLockBusinessLogic;

    //! The volume extension area
    MApplicationExtensionArea *volumeExtensionArea;

    //! Context item for getting information about video recording status
    QSharedPointer<ContextItem> useMode;

#ifdef HAVE_QMSYSTEM
    //! QmSystem watcher for device lock
    MeeGo::QmLocks qmLocks;
#endif

#ifdef UNIT_TEST
    friend class Ut_Sysuid;
#endif
};

#endif // SYSUID_H
