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

#ifndef LOCKSCREENSTATUSAREAVIEW_H
#define LOCKSCREENSTATUSAREAVIEW_H

#include <MWidgetView>
#include "lockscreenstatusareastyle.h"
#include "statusareamodel.h"
#include "contextframeworkcontext.h"

class StatusArea;
class StatusIndicator;

/*!
 * Status area view draws the status area.
 */
class LockScreenStatusAreaView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(StatusAreaModel, LockScreenStatusAreaStyle)

public:
   /*!
     * Constructs a view for a status area in the lock screen.
     *
     * \param controller the controller of this LockScreenStatusAreaView
     */
    LockScreenStatusAreaView(StatusArea *controller);

    /*!
     * Destructor.
     */
    virtual ~LockScreenStatusAreaView();

private:
    //! Context framework application context for the indicators
    ContextFrameworkContext contextFrameworkContext;

    //! The controller instance
    StatusArea *controller;

    //! Phone network indicator
    StatusIndicator *phoneNetworkIndicator;

    //! Battery indicator
    StatusIndicator *batteryIndicator;

    //! Phone signal strength indicator
    StatusIndicator *phoneSignalStrengthIndicator;

    //! Phone network type indicator
    StatusIndicator *phoneNetworkTypeIndicator;
};

#endif // STATUSAREAVIEW_H
