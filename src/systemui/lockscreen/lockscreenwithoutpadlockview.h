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
#ifndef LOCKSCREENWITHOUTPADLOCKVIEW_H
#define LOCKSCREENWITHOUTPADLOCKVIEW_H

#include "lockscreenview.h"

class NotificationArea;

class LockScreenWithoutPadlockView : public LockScreenView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MSceneWindowStyle)

public:
    /*!
     * \brief Constructs a new LockScreenWithoutPadlockView.
     *
     * \param controller the controller this is a view for
     */
    LockScreenWithoutPadlockView (MSceneWindow* controller);

    /*!
     * \brief Destroys the LockScreenWithoutPadlockView.
     */
    ~LockScreenWithoutPadlockView ();

private:

    //! Notification area
    NotificationArea* notificationArea;
};

#endif
