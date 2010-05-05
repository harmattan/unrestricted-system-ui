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
#include <QDBusConnection>
#include <MDeviceProfile>
#include <MViewCreator>

StatusAreaView::StatusAreaView(StatusArea *controller) :
    MWidgetView(controller),
    controller(controller),
    landscapeWidget(new QGraphicsWidget),
    portraitWidget(new QGraphicsWidget),
    landscapePhoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    landscapeAlarmIndicator(new ClockAlarmStatusIndicator(contextFrameworkContext, controller)),
    portraitAlarmIndicator(new ClockAlarmStatusIndicator(contextFrameworkContext, controller)),
    landscapeBatteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    portraitBatteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    landscapePhoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    landscapeInternetConnectionIndicator(new InternetConnectionStatusIndicator(contextFrameworkContext, controller)),
    portraitInternetConnectionIndicator(new InternetConnectionStatusIndicator(contextFrameworkContext, controller)),
    landscapeBluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    portraitBluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    landscapeInputMethodIndicator(new InputMethodStatusIndicator(controller)),
    landscapeNotifier(new Notifier(controller)),
    portraitNotifier(new Notifier(controller))
{
    // Set up notifiers
    landscapeNotifier->setObjectName("Notifier");
    portraitNotifier->setObjectName("Notifier");

    // Set up landscape and portrait widgets and anchor them on top of each other
    landscapeWidget->setLayout(createLandscapeLayout());
    portraitWidget->setLayout(createPortraitLayout());
    QGraphicsAnchorLayout *compositeLayout = new QGraphicsAnchorLayout;
    compositeLayout->setContentsMargins(0, 0, 0, 0);
    compositeLayout->setSpacing(0);
    compositeLayout->addCornerAnchors(landscapeWidget, Qt::TopLeftCorner, compositeLayout, Qt::TopLeftCorner);
    compositeLayout->addCornerAnchors(landscapeWidget, Qt::TopRightCorner, compositeLayout, Qt::TopRightCorner);
    compositeLayout->addCornerAnchors(portraitWidget, Qt::TopLeftCorner, landscapeWidget, Qt::BottomLeftCorner);
    compositeLayout->addCornerAnchors(portraitWidget, Qt::BottomLeftCorner, compositeLayout, Qt::BottomLeftCorner);
    controller->setLayout(compositeLayout);

    // Connect to D-Bus and register the DBus source as an object
    QDBusConnection::sessionBus().registerService("com.meego.core.MInputMethodStatusIndicator");
    QDBusConnection::sessionBus().registerObject("/inputmethodstatusindicator", landscapeInputMethodIndicator);
}

StatusAreaView::~StatusAreaView()
{
}

void StatusAreaView::setGeometry(const QRectF &rect)
{
    int screenWidth = MDeviceProfile::instance()->resolution().width();
    int screenHeight = MDeviceProfile::instance()->resolution().height();
    qreal areaHeight = rect.height() / 2;
    landscapeWidget->setMinimumSize(screenWidth, areaHeight);
    landscapeWidget->setMaximumSize(screenWidth, areaHeight);
    portraitWidget->setMinimumSize(screenHeight, areaHeight);
    portraitWidget->setMaximumSize(screenHeight, areaHeight);
}

QGraphicsLinearLayout* StatusAreaView::createLandscapeLayout()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Put indicators into the layout
    layout->addItem(landscapeBatteryIndicator);
    layout->addItem(landscapePhoneSignalStrengthIndicator);
    layout->addItem(landscapePhoneNetworkIndicator);
    layout->addStretch();
    layout->addItem(landscapeNotifier);
    layout->addStretch();
    layout->addItem(landscapeInternetConnectionIndicator);
    layout->addItem(landscapeBluetoothIndicator);
    layout->addItem(landscapeInputMethodIndicator);
    layout->addItem(createClockAlarmWidget(landscapeAlarmIndicator));

    return layout;
}

QGraphicsLinearLayout* StatusAreaView::createPortraitLayout()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Put indicators into the layout
    layout->addItem(portraitBatteryIndicator);
    layout->addItem(portraitPhoneSignalStrengthIndicator);
    layout->addItem(portraitPhoneNetworkIndicator);
    layout->addStretch();
    layout->addItem(portraitNotifier);
    layout->addStretch();
    layout->addItem(portraitInternetConnectionIndicator);
    layout->addItem(portraitBluetoothIndicator);
    layout->addItem(createClockAlarmWidget(portraitAlarmIndicator));

    return layout;
}

MWidget *StatusAreaView::createClockAlarmWidget(StatusIndicator *alarmIndicator)
{
    Clock *clock = new Clock;
    QGraphicsAnchorLayout *clockAlarmLayout = new QGraphicsAnchorLayout;
    clockAlarmLayout->setContentsMargins(0, 0, 0, 0);
    clockAlarmLayout->setSpacing(0);
    clockAlarmLayout->addCornerAnchors(clock, Qt::TopLeftCorner, clockAlarmLayout, Qt::TopLeftCorner);
    clockAlarmLayout->addCornerAnchors(clock, Qt::TopRightCorner, clockAlarmLayout, Qt::TopRightCorner);
    clockAlarmLayout->addCornerAnchors(alarmIndicator, Qt::TopRightCorner, clock, Qt::TopRightCorner);
    MWidget *clockAlarmWidget = new MWidget;
    clockAlarmWidget->setLayout(clockAlarmLayout);
    return clockAlarmWidget;
}

M_REGISTER_VIEW_NEW(StatusAreaView, StatusArea)
