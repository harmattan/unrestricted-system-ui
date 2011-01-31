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
class ScreenLockBusinessLogicAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.system_ui.request")

public:
    /*!
     * Creates a screen lock business logic adaptor.
     *
     * \param obj the parent object
     * \param screenLockBusinessLogic screen lock business logic controlled by this adaptor
     */
    ScreenLockBusinessLogicAdaptor(QObject *obj, ScreenLockBusinessLogic &screenLockBusinessLogic);

    /*!
     * Destroys the adaptor.
     */
    ~ScreenLockBusinessLogicAdaptor();

public slots:
    /*!
     * Shows the screen lock window or the event eater window.
     *
     * \param service DBus service to call when unlock is performed.
     * \param path DBus path to call when unlock is performed.
     * \param interface DBus interface to call when unlock is performed.
     * \param method DBus method to call when unlock is performed.
     * \param mode The ScreenLockBusinessLogicAdaptor::TkLockMode opcode.
     * \param silent Whether to show a notification or not (deprecated)
     * \param flicker Deprecated/not used
     */
    int tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker);

    /*!
     * Hides the screen lock.
     *
     * \param silent Whether to show notifications or not (deprecated).
     */
    int tklock_close(bool silent);

private slots:
    //! Calls the MCE's unlock callback function.
    void callUnlockCallback();

    //! Shows the screen lock window and hides the event eater window.
    void enableVisual();

    //! Shows the event eater window.
    void enableEater();

    //! Hides the event eater window and the screen lock window.
    void hideVisualAndEater();

private:
    enum {
        TkLockReplyFailed = 0,
        TkLockReplyOk
    } TklockReply;

    enum {
        TkLockModeNone,     // Deprecated
        TkLockModeEnable,   // Show the lock UI in lock mode
        TkLockModeHelp,     // Deprecated
        TkLockModeSelect,   // Deprecated
        TkLockModeOneInput, // Turn the event eater on
        TkLockEnableVisual  // Show unlock UI
    } TkLockMode;

    enum {
        TkLockUnlock = 1,
        TkLockRetry,
        TkLockTimeout,
        TkLockClosed
    } TkLockStatus;

    //! Screen lock business logic connected to this D-Bus adaptor
    ScreenLockBusinessLogic &screenLockBusinessLogic;

    // MCE callback D-Bus interface
    QDBusInterface *callbackInterface;

    // Name of the MCE callback method
    QString callbackMethod;

#ifdef UNIT_TEST
    friend class Ut_LockScreenBusinessLogicAdaptor;
#endif
};

#endif 
