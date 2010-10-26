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

class UsbUi;
class BatteryBusinessLogic;
class ShutdownBusinessLogic;
class StatusAreaRenderer;
class StatusIndicatorMenuWindow;
class NotificationManager;
class MCompositorNotificationSink;
class NGFNotificationSink;
class UnlockNotificationSink;
class SysUidRequest;
class VolumeBarWindow;

class Sysuid : public QObject
{
    Q_OBJECT

public:
    Sysuid(QObject* parent);
    virtual ~Sysuid();
    static Sysuid* instance();
    static QString dbusService();
    static QString dbusPath();

    /*!
     * Returns a reference to the notification manager.
     *
     * \return a reference to the notification manager
     */
    NotificationManager &notificationManager();

    /*!
     * Returns a reference to the compositor notification sink.
     *
     * \return a reference to the compositor notification sink
     */
    MCompositorNotificationSink& compositorNotificationSink();

    /*!
     * Returns a reference to the unlock-screen notification sink.
     *
     * \return a reference to the unlock-screen notification sink
     */
    UnlockNotificationSink& unlockNotificationSink();

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
    StatusIndicatorMenuWindow *statusIndicatorMenuWindow;

    //! Notification manager
    NotificationManager *notificationManager_;

    //! Compositor notification sink for visualizing the notification outside home
    MCompositorNotificationSink *mCompositorNotificationSink;

    //! Feedback notification sink for presenting the notification as a feedback
    NGFNotificationSink *ngfNotificationSink;

    //! Unlock screen notification sink for presenting missed events on ui
    UnlockNotificationSink *unlockNotificationSink_;

    //! The system-bus adaptor class for unlock-screen interface
    SysUidRequest *sysUidRequest;

    //! The volume-control ui
    VolumeBarWindow *volumeBarWindow;

    //! Context item for getting information about video recording status
    QSharedPointer<ContextItem> useMode;

#ifdef UNIT_TEST
    friend class Ut_Sysuid;
#endif
};

#endif // SYSUID_H
