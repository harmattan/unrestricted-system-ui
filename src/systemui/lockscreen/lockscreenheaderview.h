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

#ifndef LOCKSCREENHEADERVIEW_H_
#define LOCKSCREENHEADERVIEW_H_

#include <MWidgetView>
#include "lockscreenheaderstyle.h"

class QGraphicsLinearLayout;

/*!
 * The feed view draws a header for the lock screen.
 */
class LockScreenHeaderView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, LockScreenHeaderStyle)

public:
    /*!
     * Constructs a LockScreenHeaderView.
     *
     * \param controller the controller to be used
     */
    LockScreenHeaderView(MWidgetController *controller);

    /*!
     * Destroys the LockScreenHeaderView.
     */
    virtual ~LockScreenHeaderView();

private:
    //! The main layout
    QGraphicsLinearLayout *layout;
};

#endif /* LOCKSCREENHEADERVIEW_H_ */
