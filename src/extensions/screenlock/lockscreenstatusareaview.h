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
 * Lock screen status area view draws the status area for the lock screen.
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

    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    //! \reimp_end

private slots:
    //! Updates the _MEEGOTOUCH_MSTATUSBAR_GEOMETRY X window property
    void updateStatusBarGeometryProperty();

private:
    //! Context framework application context for the indicators
    ContextFrameworkContext contextFrameworkContext;

    //! The controller instance
    StatusArea *controller;

    //! Phone network and Internet connection indicator
    StatusIndicator *phoneNetworkIndicator;

    //! Battery indicator
    StatusIndicator *batteryIndicator;

    //! Phone signal strength indicator
    StatusIndicator *phoneSignalStrengthIndicator;

    //! Phone network type indicator
    StatusIndicator *phoneNetworkTypeIndicator;

    //! Bluetooth indicator
    StatusIndicator *bluetoothIndicator;

    //! GPS indicator
    StatusIndicator *gpsIndicator;

    //! Presence indicator
    StatusIndicator *presenceIndicator;

    //! Profile indicator
    StatusIndicator *profileIndicator;

    //! Call indicator
    StatusIndicator *callIndicator;

    //! Alarm indicator
    StatusIndicator *alarmIndicator;

    //! Notification notifier
    StatusIndicator *notifierIndicator;

    //! Call forwarding indicator
    StatusIndicator *callForwardingIndicator;

    //! Transfer status indicator
    StatusIndicator *transferStatusIndicator;

    //! The status bar geometry that has been put into the _MEEGOTOUCH_MSTATUSBAR_GEOMETRY property
    QRectF updatedStatusBarGeometry;

    //! Whether the orientation change signal has been connected or not
    bool orientationChangeSignalConnected;

#ifdef UNIT_TEST
    friend class Ut_LockScreenStatusAreaView;
#endif
};

#endif // LOCKSCREENSTATUSAREAVIEW_H
