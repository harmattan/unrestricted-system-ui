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
    /*!
     * Destructor.
     */
    virtual ~ScreenLockExtensionInterface() {}

    /*!
     * Resets the screen lock extension to its initial state.
     */
    virtual void reset() = 0;
};

Q_DECLARE_INTERFACE(ScreenLockExtensionInterface, "com.meego.core.ScreenLockExtensionInterface/1.0")

#endif // SCREENLOCKEXTENSIONINTERFACE_H
