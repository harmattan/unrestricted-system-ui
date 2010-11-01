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
#ifndef LOCKSSTUB_H
#define LOCKSSTUB_H

#include <QObject>

// STUB CLASS

namespace MeeGo
{

/**
 * @brief Provides information and actions on device locks.
 */
class QmLocks : public QObject
{
    Q_OBJECT

public:
    /** Things that can be locked. */
    enum Lock
    {
        Device = 0,       /**< Device lock */
        TouchAndKeyboard  /**< Touchscreen and keyboard lock */
    };

    /** Lock states */
    enum State
    {
        Unlocked = 0,     /**< Unlocked */
        Locked            /**< Locked */
    };


public:
    QmLocks(QObject *parent = 0);
    ~QmLocks();

    /**
     * Get the current lock state.
     * @param what Which lock state to request.
     * @return Current lock state for @c what.
     */
    QmLocks::State getState(QmLocks::Lock what) const;


    /**
     * Set current lock state. Note that this interface does not allow
     * to unlock device.
     * <p><b>Credentials</b>:<br>
     *   <em>write-device-data</em> Resource token required to set the device lock state.
     * @param what Which lock state to set
     * @param how Set @c to which state.
     * @return True on success, false otherwise.
     */
    bool setState(QmLocks::Lock what, QmLocks::State how);

    /**
     * Set device autolock timeout. Device is automatically locked
     * after the specified amount of inactivity.
     * <p><b>Credentials</b>:<br>
     *   <em>lock-control</em> Resource token required to set the timeout.
     * @param seconds Number of seconds of inactivity after which the device is automatically locked. Value <=0 disables autolock.
     * @return True on success, false otherwise.
     */
    bool setDeviceAutolockTime(int seconds);

    /**
     * Get device autolock timeout. 
     * @return Timeout in seconds, value 0 means that autolock is not acticated, -1 is error
     */
    int getDeviceAutolockTime();

signals:
    /**
     * Sent when some device lock state has changed.
     * @param what Which lock state was changed
     * @param how Which state the lock was changed to.
     */
    void stateChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
};
}

#endif // LOCKSSTUB_H

