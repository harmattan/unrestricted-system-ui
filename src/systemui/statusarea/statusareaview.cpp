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
#include "clock.h"
#include "contextframeworkcontext.h"
#include "notificationstatusindicator.h"
#include "notifiernotificationsink.h"
#include "inputmethodstatusindicatoradaptor.h"
#include "sysuid.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <QDBusConnection>
#include <MDeviceProfile>
#include <MViewCreator>
#include <MStylableWidget>

StatusAreaView::StatusAreaView(StatusArea *controller) :
    MWidgetView(controller),
    landscapeWidget(new MStylableWidget),
    portraitWidget(new MStylableWidget),
    controller(controller),
    landscapePhoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    landscapeAlarmIndicator(new AlarmStatusIndicator(contextFrameworkContext, controller)),
    portraitAlarmIndicator(new AlarmStatusIndicator(contextFrameworkContext, controller)),
    landscapeBatteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    portraitBatteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    landscapePhoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    landscapePhoneNetworkTypeIndicator(new PhoneNetworkTypeStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneNetworkTypeIndicator(new PhoneNetworkTypeStatusIndicator(contextFrameworkContext, controller)),
    landscapeBluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    portraitBluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    landscapeGPSIndicator(new GPSStatusIndicator(contextFrameworkContext, controller)),
    portraitGPSIndicator(new GPSStatusIndicator(contextFrameworkContext, controller)),
    landscapePresenceIndicator(new PresenceStatusIndicator(contextFrameworkContext, controller)),
    portraitPresenceIndicator(new PresenceStatusIndicator(contextFrameworkContext, controller)),
    landscapeProfileIndicator(new ProfileStatusIndicator(contextFrameworkContext, controller)),
    portraitProfileIndicator(new ProfileStatusIndicator(contextFrameworkContext, controller)),
    landscapeInputMethodIndicator(new InputMethodStatusIndicator(controller)),
    landscapeCallIndicator(new CallStatusIndicator(contextFrameworkContext, controller)),
    portraitCallIndicator(new CallStatusIndicator(contextFrameworkContext, controller)),
    landscapeNotificationIndicator(new NotificationStatusIndicator(controller)),
    portraitNotificationIndicator(new NotificationStatusIndicator(controller)),
    landscapeCallForwardingIndicator(new CallForwardingStatusIndicator(contextFrameworkContext, controller)),
    portraitCallForwardingIndicator(new CallForwardingStatusIndicator(contextFrameworkContext, controller)),
    landscapeTransferStatusIndicator(new TransferStatusIndicator(controller)),
    portraitTransferStatusIndicator(new TransferStatusIndicator(controller)),
    landscapeClock(new Clock(controller)),
    portraitClock(new Clock(controller))
{
    // Connect related phone network indicators
    connect(portraitPhoneSignalStrengthIndicator, SIGNAL(networkAvailabilityChanged(bool)), portraitPhoneNetworkTypeIndicator, SLOT(setNetworkAvailability(bool)));
    connect(landscapePhoneSignalStrengthIndicator, SIGNAL(networkAvailabilityChanged(bool)), landscapePhoneNetworkTypeIndicator, SLOT(setNetworkAvailability(bool)));

    // Set up the clock styles
    landscapeClock->setObjectName("StatusAreaLandscapeClock");
    portraitClock->setObjectName("StatusAreaPortraitClock");

    // Set up landscape and portrait widgets and anchor them on top of each other
    landscapeWidget->setStyleName("StatusBarLandscapeWidget");
    landscapeWidget->setLayout(createLandscapeLayout());
    landscapeWidget->setParent(controller);
    portraitWidget->setStyleName("StatusBarPortraitWidget");
    portraitWidget->setLayout(createPortraitLayout());
    portraitWidget->setParent(controller);
    QGraphicsAnchorLayout *compositeLayout = new QGraphicsAnchorLayout;
    compositeLayout->setContentsMargins(0, 0, 0, 0);
    compositeLayout->setSpacing(0);

    compositeLayout->addCornerAnchors(landscapeWidget, Qt::TopLeftCorner, compositeLayout, Qt::TopLeftCorner);
    compositeLayout->addCornerAnchors(landscapeWidget, Qt::TopRightCorner, compositeLayout, Qt::TopRightCorner);
    compositeLayout->addCornerAnchors(portraitWidget, Qt::TopLeftCorner, landscapeWidget, Qt::BottomLeftCorner);
    compositeLayout->addCornerAnchors(portraitWidget, Qt::BottomLeftCorner, compositeLayout, Qt::BottomLeftCorner);
    controller->setLayout(compositeLayout);

    // Connect to D-Bus and register the DBus source as an object
    new InputMethodStatusIndicatorAdaptor(landscapeInputMethodIndicator);
    QDBusConnection::sessionBus().registerService("com.meego.core.MInputMethodStatusIndicator");
    QDBusConnection::sessionBus().registerObject("/inputmethodstatusindicator", landscapeInputMethodIndicator);

    // Set different style names to PhoneNetworkStatusIndicator in landscape and portrait
    landscapePhoneNetworkIndicator->setStyleName(QString(landscapePhoneNetworkIndicator->metaObject()->className()) + "Landscape");
    portraitPhoneNetworkIndicator->setStyleName(QString(portraitPhoneNetworkIndicator->metaObject()->className()) + "Portrait");

    // Connect notification signals
    connect(&Sysuid::instance()->notifierNotificationSink(), SIGNAL(notifierSinkActive(bool)), landscapeNotificationIndicator, SLOT(setActive(bool)));
    connect(&Sysuid::instance()->notifierNotificationSink(), SIGNAL(notifierSinkActive(bool)), portraitNotificationIndicator, SLOT(setActive(bool)));

    // Set up the class for functional testing
    setupTestability();
}

void StatusAreaView::setupTestability()
{
    portraitPhoneNetworkIndicator->setParent(portraitWidget);
    landscapeAlarmIndicator->setParent(landscapeWidget);
    portraitAlarmIndicator->setParent(portraitWidget);
    landscapeBatteryIndicator->setParent(landscapeWidget);
    portraitBatteryIndicator->setParent(portraitWidget);
    landscapePhoneSignalStrengthIndicator->setParent(landscapeWidget);
    portraitPhoneSignalStrengthIndicator->setParent(portraitWidget);
    landscapePhoneNetworkTypeIndicator->setParent(landscapeWidget);
    portraitPhoneNetworkTypeIndicator->setParent(portraitWidget);
    landscapeBluetoothIndicator->setParent(landscapeWidget);
    portraitBluetoothIndicator->setParent(portraitWidget);
    landscapeGPSIndicator->setParent(landscapeWidget);
    portraitGPSIndicator->setParent(portraitWidget);
    landscapePresenceIndicator->setParent(landscapeWidget);
    portraitPresenceIndicator->setParent(portraitWidget);
    landscapeProfileIndicator->setParent(landscapeWidget);
    portraitProfileIndicator->setParent(portraitWidget);
    landscapeInputMethodIndicator->setParent(landscapeWidget);
    landscapeCallIndicator->setParent(landscapeWidget);
    portraitCallIndicator->setParent(portraitWidget);
    landscapeNotificationIndicator->setParent(landscapeWidget);
    portraitNotificationIndicator->setParent(portraitWidget);
    landscapeClock->setParent(landscapeWidget);
    portraitClock->setParent(portraitWidget);
    landscapeCallForwardingIndicator->setParent(landscapeWidget);
    portraitCallForwardingIndicator->setParent(portraitWidget);
    landscapeTransferStatusIndicator->setParent(landscapeWidget);
    portraitTransferStatusIndicator->setParent(portraitWidget);
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
    layout->addItem(landscapePhoneNetworkTypeIndicator);
    layout->addStretch();
    layout->addItem(landscapeNotificationIndicator);
    layout->addItem(landscapeTransferStatusIndicator);
    layout->addItem(landscapeCallForwardingIndicator);
    layout->addItem(landscapeBluetoothIndicator);
    layout->addItem(landscapeGPSIndicator);
    layout->addItem(landscapePresenceIndicator);
    layout->addItem(landscapeProfileIndicator);
    layout->addItem(landscapeInputMethodIndicator);
    layout->addItem(landscapeCallIndicator);
    layout->addItem(landscapeAlarmIndicator);
    layout->addItem(landscapeClock);

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
    layout->addItem(portraitPhoneNetworkTypeIndicator);
    layout->addStretch();
    layout->addItem(portraitNotificationIndicator);
    layout->addItem(portraitTransferStatusIndicator);
    layout->addItem(portraitCallForwardingIndicator);
    layout->addItem(portraitBluetoothIndicator);
    layout->addItem(portraitGPSIndicator);
    layout->addItem(portraitPresenceIndicator);
    layout->addItem(portraitProfileIndicator);
    layout->addItem(portraitCallIndicator);
    layout->addItem(portraitAlarmIndicator);
    layout->addItem(portraitClock);

    return layout;
}

M_REGISTER_VIEW_NEW(StatusAreaView, StatusArea)
