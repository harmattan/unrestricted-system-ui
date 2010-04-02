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
#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>
#include "duinamespace.h"
#include <QSharedPointer>
#include "applicationcontext.h"

class UsbUi;
class SystemUIGConf;
class BatteryBusinessLogic;
class LedBusinessLogic;
class ShutdownBusinessLogic;
class StatusAreaWindow;
class NotificationManager;
class DuiCompositorNotificationSink;
class DuiFeedbackNotificationSink;
class DuiApplicationWindow;

class Sysuid : public QObject
{
    Q_OBJECT

public:
    Sysuid ();
    virtual ~Sysuid ();
    static Sysuid* sysuid ();
    static QString dbusService ();
    static QString dbusPath ();

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
    DuiCompositorNotificationSink& compositorNotificationSink();

    /*!
     * Returns the current orientation of the status area window's scene.
     */
    virtual Dui::Orientation orientation() const;

    /*!
     * Returns the current orientation angle of the status area window's scene.
     */
    virtual Dui::OrientationAngle orientationAngle() const;

    /*!
     * Returns a reference to the application window.
     *
     * \return a reference to the application window
     */
    DuiApplicationWindow &applicationWindow();

signals:
    /*!
      * Inform about orientation changes
      */
    void orientationChangeFinished(const Dui::Orientation &);

public slots:
    void retranslate ();

private slots:
    /*!
     * Enables or disables various sinks according to current use mode
     */
    void applyUseMode();

private:
    SystemUIGConf           *m_SystemUIGConf;
    BatteryBusinessLogic    *m_BatteryLogic;
    LedBusinessLogic        *m_LedLogic;
    ShutdownBusinessLogic   *m_ShutdownLogic;
    UsbUi                   *m_UsbUi;
    static Sysuid           *m_Sysuid;

    //! Status area window
    StatusAreaWindow        *m_statusAreaWindow;

    //! Application window
    DuiApplicationWindow    *m_applicationWindow;

    //! Notification manager
    NotificationManager     *m_notificationManager;

    //! Compositor notification sink for visualizing the notification outside home
    DuiCompositorNotificationSink *m_compositorNotificationSink;

    //! Feedback notification sink for presenting the notification as a feedback
    DuiFeedbackNotificationSink   *m_feedbackNotificationSink;

    //! Context item for getting information about video recording status
    QSharedPointer<ContextItem> useMode;
};

#endif // SYSUID_H
