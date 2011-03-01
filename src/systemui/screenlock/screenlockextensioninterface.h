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

#ifndef SCREENLOCKEXTENSIONINTERFACE_H
#define SCREENLOCKEXTENSIONINTERFACE_H

#include <QObject>
#include <mapplicationextensioninterface.h>

class NotificationManagerInterface;

/**
 * ScreenLockExtensionInterface is the base class for screen lock extension classes.
 *
 * Extension developers need to implement this interface in their extension binary
 * and export their implementation from the binary. The host application will maintain
 * an inventory of available extensions and instantiate them using this interface
 * when needed.
 */
class ScreenLockExtensionInterface : public MApplicationExtensionInterface
{
public:
    //! Screen lock modes
    enum ScreenLockMode {
        NormalMode,
        LowPowerMode,
        DisplayOffMode
    };

    /*!
     * Destructor.
     */
    virtual ~ScreenLockExtensionInterface() {}

    /*!
     * Resets the screen lock extension to its initial state.
     */
    virtual void reset() = 0;

    /*!
     * Sets the notification manager interface to be used in the extension.
     *
     * \param notificationManager the interface to be used in the extension
     */
    virtual void setNotificationManagerInterface(NotificationManagerInterface &notificationManager) = 0;

    /*!
     * Returns the QObject that implements the interface.
     *
     * \return the QObject that implements the interface
     */
    virtual QObject *qObject() = 0;

    /*!
     * Sets the mode of the screen lock.
     *
     * \param mode the screen lock mode
     */
    virtual void setMode(ScreenLockMode mode) = 0;
};

Q_DECLARE_INTERFACE(ScreenLockExtensionInterface, "com.meego.core.ScreenLockExtensionInterface/0.20")

#endif // SCREENLOCKEXTENSIONINTERFACE_H
