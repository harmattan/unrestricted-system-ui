/***************************************************************************
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

#ifndef LOCKSCREENHEADERWITHPADLOCKVIEW_H_
#define LOCKSCREENHEADERWITHPADLOCKVIEW_H_

#include "lockscreenheaderview.h"

class MImageWidget;

/*!
 * The LockScreenHeaderWithPadlockView class draws header containing a padlock for the lock screen
 */
class LockScreenHeaderWithPadlockView : public LockScreenHeaderView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, LockScreenHeaderStyle)

public:
    /*!
     * Constructs a LockScreenHeaderWithPadlockView.
     *
     * \param controller the controller to be used
     */
    LockScreenHeaderWithPadlockView(MWidgetController *controller);

    /*!
     * Destroys the LockScreenHeaderWithPadlockView.
     */
    virtual ~LockScreenHeaderWithPadlockView();

private:
    //! Image widget used to show the dragable pad lock
    MImageWidget *padlockImageWidget;
};

#endif /* LOCKSCREENHEADERWITHPADLOCKVIEW_H_ */
