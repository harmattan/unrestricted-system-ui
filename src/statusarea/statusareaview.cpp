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

#include "statusareaview.h"
#include "statusarea.h"
#include "statusindicator.h"
#include "notifier.h"
#include "clock.h"
#include "contextframeworkcontext.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <MSceneWindow>
#include <MSceneManager>
#include <MViewCreator>

StatusAreaView::StatusAreaView(StatusArea *controller) :
    MWidgetView(controller),
    controller(controller),
    alarmIndicator(new ClockAlarmStatusIndicator(contextFrameworkContext, controller)),
    batteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    phoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    internetConnectionIndicator(new InternetConnectionStatusIndicator(contextFrameworkContext, controller)),
    bluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    notifier(new Notifier(controller)),
    portraitAlarmIndicator(new ClockAlarmStatusIndicator(contextFrameworkContext, controller)),
    portraitBatteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    portraitInternetConnectionIndicator(new InternetConnectionStatusIndicator(contextFrameworkContext, controller)),
    portraitBluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    portraitNotifier(new Notifier(controller))
{
    QGraphicsLinearLayout *landscapeLayout = setupLandscapeLayout();
    QGraphicsLinearLayout *portraitLayout = setupPortraitLayout();
    QGraphicsLinearLayout *compositeLayout = new QGraphicsLinearLayout(Qt::Vertical);

    compositeLayout->setContentsMargins(0, 0, 0, 0);
    compositeLayout->setSpacing(0);
    compositeLayout->addItem(landscapeLayout);
    compositeLayout->addItem(portraitLayout);
    controller->setLayout(compositeLayout);
}

QGraphicsLinearLayout* StatusAreaView::setupLandscapeLayout()
{
    // Layout for clock and search button
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create a composite widget with a clock and the alarm indicator
    Clock *clock = new Clock;
    QGraphicsAnchorLayout *clockAlarmLayout = new QGraphicsAnchorLayout;
    clockAlarmLayout->setContentsMargins(0, 0, 0, 0);
    clockAlarmLayout->addCornerAnchors(clock, Qt::TopLeftCorner, clockAlarmLayout, Qt::TopLeftCorner);
    clockAlarmLayout->addCornerAnchors(clock, Qt::TopRightCorner, clockAlarmLayout, Qt::TopRightCorner);
    clockAlarmLayout->addCornerAnchors(alarmIndicator, Qt::TopRightCorner, clock, Qt::TopRightCorner);
    MWidget *clockAlarmWidget = new MWidget;
    clockAlarmWidget->setLayout(clockAlarmLayout);
    layout->addItem(clockAlarmWidget);

    // Put the rest of the indicators to the layout
    layout->addItem(batteryIndicator);
    layout->addItem(phoneSignalStrengthIndicator);
    layout->addItem(internetConnectionIndicator);
    layout->addItem(bluetoothIndicator);

    // Create the notifier
    notifier->setObjectName("Notifier");
    layout->addItem(notifier);

    // Stretch
    layout->addStretch();

    return layout;
}

QGraphicsLinearLayout* StatusAreaView::setupPortraitLayout()
{
    QGraphicsLinearLayout *portraitLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    portraitLayout->setContentsMargins(0, 0, 0, 0);
    portraitLayout->setSpacing(0);

    Clock *portraitClock = new Clock;
    QGraphicsAnchorLayout *portraitClockAlarmLayout = new QGraphicsAnchorLayout;
    portraitClockAlarmLayout->setContentsMargins(0, 0, 0, 0);
    portraitClockAlarmLayout->addCornerAnchors(portraitClock, Qt::TopLeftCorner, portraitClockAlarmLayout, Qt::TopLeftCorner);
    portraitClockAlarmLayout->addCornerAnchors(portraitClock, Qt::TopRightCorner, portraitClockAlarmLayout, Qt::TopRightCorner);
    portraitClockAlarmLayout->addCornerAnchors(portraitAlarmIndicator, Qt::TopRightCorner, portraitClock, Qt::TopRightCorner);
    MWidget *portraitClockAlarmWidget = new MWidget;
    portraitClockAlarmWidget->setLayout(portraitClockAlarmLayout);
    portraitLayout->addItem(portraitClockAlarmWidget);

    portraitLayout->addItem(portraitBatteryIndicator);
    portraitLayout->addItem(portraitPhoneSignalStrengthIndicator);
    portraitLayout->addItem(portraitInternetConnectionIndicator);
    portraitLayout->addItem(portraitBluetoothIndicator);

    // Create the notifier
    notifier->setObjectName("Notifier");
    portraitLayout->addItem(portraitNotifier);

    // Stretch
    portraitLayout->addStretch();

    return portraitLayout;
}

StatusAreaView::~StatusAreaView()
{
}

M_REGISTER_VIEW_NEW(StatusAreaView, StatusArea)
