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
#ifndef SYSUIDREQUEST_H
#define SYSUIDREQUEST_H

#include <QObject>

class ScreenLockBusinessLogic;
class QDBusConnection;

/*!
 * This class creates a connection between the system bus and the methods
 * implemented in the ScreenLockBusinessLogicAdaptor where the methods
 * implemented for the MCE handling the locking of the touch screen.
 */
class SysUidRequest : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs a SysUidRequest instance.
     */
    SysUidRequest();

    //! Returns an instance of the lock screen business logic
    ScreenLockBusinessLogic *screenLockBusinessLogic();

private:
    //! The lock screen business logic
    ScreenLockBusinessLogic *screenLockBusinessLogic_;
};

#endif
