/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#ifndef LOCKSCREENBUSINESSLOGICADAPTOR_H
#define LOCKSCREENBUSINESSLOGICADAPTOR_H

#include "lockscreenbusinesslogic.h"

#include <QDBusAbstractAdaptor>

/*!
 * Implements the DBus API interface for the crceen locking mechanism.
 */
class LockScreenBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    #if 1
    // This is what used in MCE.
    Q_CLASSINFO("D-Bus Interface", "com.nokia.system_ui.request")
    #else
    // This is what others want.
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.LockScreen")
    #endif

public:
    LockScreenBusinessLogicAdaptor (
            QObject *                obj, 
            LockScreenBusinessLogic *logic);

    /*
     * I have no information about these, copyed from osso-systemui-tklock.
     */
    enum {
        TkLockReplyFailed = 0,
        TkLockReplyOk     = 1
    } TklockReply;

    enum {
        TkLockModeNone,      // Unused, maybe deprecated
        TkLockModeEnable,    // Show the lock UI in lock mode
        TkLockModeHelp,      // Unused, deprecated
        TkLockModeSelect,    // Unused, deprecated
        TkLockModeOneInput,  // Turn the event eater on
        TkLockEnableVisual   // Show unlock ui
    } TkLockMode;
    
public slots:
    Q_NOREPLY void SetMissedEvents (
            int emails, 
            int messages, 
            int calls, 
            int im);

    int tklock_open (
            const QString  &a,
            const QString  &b,
            const QString  &c,
            const QString  &d,
            uint       u,
            bool       b1,
            bool       b2);

    /*
     * This is not working. 
     */
    int tklock_close (bool b);

private slots:
    void unlockConfirmed ();

signals:
    void delegateSetMissedEvents (int, int, int, int);

private:
    LockScreenBusinessLogic   *m_LockScreenBusinessLogic;
    QString                    m_MCECallbackService;
    QString                    m_MCECallbackPath;
    QString                    m_MCECallbackInterface;
    QString                    m_MCECallbackMethod;
};

#endif 
