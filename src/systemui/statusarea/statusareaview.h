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

#ifndef STATUSAREAVIEW_H
#define STATUSAREAVIEW_H

#include <MWidgetView>
#include "statusareastyle.h"
#include "statusareamodel.h"
#include "contextframeworkcontext.h"

class Clock;
class StatusArea;
class StatusIndicator;
class InputMethodStatusIndicator;
class QGraphicsLinearLayout;

/*!
 * Status area view draws the status area.
 */
class StatusAreaView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(StatusAreaModel, StatusAreaStyle)

public:
   /*!
     * Constructs a view for a status area.
     *
     * \param controller the controller of this StatusAreaView
     */
    StatusAreaView(StatusArea *controller);

    /*!
     * Destructor.
     */
    virtual ~StatusAreaView();

    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    //! \reimp_end

protected:
    //! Widgets for the landscape and portrait areas
    MWidgetController *landscapeWidget;
    MWidgetController *portraitWidget;

private:
    //! Context framework application context for the indicators
    ContextFrameworkContext contextFrameworkContext;

    //! The controller instance
    StatusArea *controller;

    //! Phone network indicator
    StatusIndicator *landscapePhoneNetworkIndicator;
    StatusIndicator *portraitPhoneNetworkIndicator;

    //! Alarm indicator
    StatusIndicator *landscapeAlarmIndicator;
    StatusIndicator *portraitAlarmIndicator;

    //! Battery indicator
    StatusIndicator *landscapeBatteryIndicator;
    StatusIndicator *portraitBatteryIndicator;

    //! Phone signal strength indicator
    StatusIndicator *landscapePhoneSignalStrengthIndicator;
    StatusIndicator *portraitPhoneSignalStrengthIndicator;

    //! Phone network type and Internet connection indicator
    StatusIndicator *landscapePhoneNetworkTypeIndicator;
    StatusIndicator *portraitPhoneNetworkTypeIndicator;

    //! Bluetooth indicator
    StatusIndicator *landscapeBluetoothIndicator;
    StatusIndicator *portraitBluetoothIndicator;

    //! GPS indicator
    StatusIndicator *landscapeGPSIndicator;
    StatusIndicator *portraitGPSIndicator;

    //! Presence indicator
    StatusIndicator *landscapePresenceIndicator;
    StatusIndicator *portraitPresenceIndicator;

    //! Profile indicator
    StatusIndicator *landscapeProfileIndicator;
    StatusIndicator *portraitProfileIndicator;

    //! Input method indicator (only in landscape)
    InputMethodStatusIndicator *landscapeInputMethodIndicator;

    //! Call indicator
    StatusIndicator *landscapeCallIndicator;
    StatusIndicator *portraitCallIndicator;

    //! Notification notifier
    StatusIndicator *landscapeNotificationIndicator;
    StatusIndicator *portraitNotificationIndicator;

    //! Call forwarding status indicator
    StatusIndicator *landscapeCallForwardingIndicator;
    StatusIndicator *portraitCallForwardingIndicator;

    //! Transfer status indicator
    StatusIndicator *landscapeTransferStatusIndicator;
    StatusIndicator *portraitTransferStatusIndicator;

    //! Clock
    Clock *landscapeClock;
    Clock *portraitClock;

    //! Create the layout for status area in landscape mode
    QGraphicsLinearLayout *createLandscapeLayout();

    //! Create the layout for status area in portrait mode
    QGraphicsLinearLayout *createPortraitLayout();

    //! Set up class for functional testing
    void setupTestability();

#ifdef UNIT_TEST
    friend class Ut_StatusAreaView;
#endif
};

#endif // STATUSAREAVIEW_H
