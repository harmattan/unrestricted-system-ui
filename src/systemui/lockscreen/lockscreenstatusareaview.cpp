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

#include "lockscreenstatusareaview.h"
#include "statusarea.h"
#include "statusindicator.h"
#include "contextframeworkcontext.h"
#include <QGraphicsLinearLayout>
#include <MViewCreator>

LockScreenStatusAreaView::LockScreenStatusAreaView(StatusArea *controller) :
    MWidgetView(controller),
    controller(controller),
    phoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    batteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    phoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    phoneNetworkTypeIndicator(new PhoneNetworkTypeStatusIndicator(contextFrameworkContext, controller)),
    internetConnectionIndicator(new InternetConnectionStatusIndicator(contextFrameworkContext, controller)),
    bluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    gpsIndicator(new GPSStatusIndicator(contextFrameworkContext, controller)),
    presenceIndicator(new PresenceStatusIndicator(contextFrameworkContext, controller)),
    profileIndicator(new ProfileStatusIndicator(contextFrameworkContext, controller)),
    callIndicator(new CallStatusIndicator(contextFrameworkContext, controller)),
    alarmIndicator(new AlarmStatusIndicator(contextFrameworkContext, controller))
{
    // Connect related phone network indicators
    connect(phoneNetworkTypeIndicator, SIGNAL(networkAvailabilityChanged(bool)), phoneSignalStrengthIndicator, SLOT(setDisplay(bool)));

    // Put indicators into the layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(batteryIndicator);
    layout->addItem(phoneSignalStrengthIndicator);
    layout->addItem(phoneNetworkTypeIndicator);
    layout->addItem(phoneNetworkIndicator);
    layout->addStretch();
    layout->addItem(internetConnectionIndicator);
    layout->addItem(bluetoothIndicator);
    layout->addItem(gpsIndicator);
    layout->addItem(presenceIndicator);
    layout->addItem(profileIndicator);
    layout->addItem(callIndicator);
    layout->addItem(alarmIndicator);
    controller->setLayout(layout);
}

LockScreenStatusAreaView::~LockScreenStatusAreaView()
{
}

M_REGISTER_VIEW_NEW(LockScreenStatusAreaView, StatusArea)
