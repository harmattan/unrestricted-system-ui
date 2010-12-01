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
#ifndef SCREENLOCKBUSINESSLOGICADAPTOR_H
#define SCREENLOCKBUSINESSLOGICADAPTOR_H

#include "screenlockbusinesslogic.h"
#include <QDBusAbstractAdaptor>

class QDBusInterface;

/*!
 * Implements the DBus API interface for the screen locking mechanism.
 */
class ScreenLockBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
#ifndef UNIT_TEST
    Q_CLASSINFO("D-Bus Interface", "com.nokia.system_ui.request")
#endif

public:
    ScreenLockBusinessLogicAdaptor (
            QObject *                obj, 
            ScreenLockBusinessLogic *logic);
    ~ScreenLockBusinessLogicAdaptor();

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
    int tklock_open (
            const QString  &a,
            const QString  &b,
            const QString  &c,
            const QString  &d,
            uint       u,
            bool       b1,
            bool       b2);

    /*
     * This is not working always.
     */
    int tklock_close (bool b);

private slots:
    void unlockConfirmed ();

    void enableVisual ();
    void enableEater ();
    void hideVisualAndEater ();

private:
    ScreenLockBusinessLogic   *m_LockScreenBusinessLogic;
    QString                    m_MCECallbackService;
    QString                    m_MCECallbackPath;
    QString                    m_MCECallbackInterface;
    QString                    m_MCECallbackMethod;
    QDBusInterface            *m_CallbackDbusIf;

#ifdef UNIT_TEST
    friend class Ut_LockScreenBusinessLogicAdaptor;
#endif
};

#endif 
