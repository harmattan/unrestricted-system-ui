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
#include "x11wrapper.h"
#include "notificationstatusindicator.h"
#include "screenlockextension.h"
#include <QGraphicsLinearLayout>
#include <QX11Info>
#include <MViewCreator>
#include <MSceneManager>

LockScreenStatusAreaView::LockScreenStatusAreaView(StatusArea *controller) :
    MWidgetView(controller),
    controller(controller),
    phoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    batteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    phoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    phoneNetworkTypeIndicator(new PhoneNetworkTypeStatusIndicator(contextFrameworkContext, controller)),
    bluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    gpsIndicator(new GPSStatusIndicator(contextFrameworkContext, controller)),
    presenceIndicator(new PresenceStatusIndicator(contextFrameworkContext, controller)),
    profileIndicator(new ProfileStatusIndicator(contextFrameworkContext, controller)),
    callIndicator(new CallStatusIndicator(contextFrameworkContext, controller)),
    alarmIndicator(new AlarmStatusIndicator(contextFrameworkContext, controller)),
    notifierIndicator(new NotificationStatusIndicator(controller)),
    callForwardingIndicator(new CallForwardingStatusIndicator(contextFrameworkContext, controller)),
    transferStatusIndicator(new TransferStatusIndicator(controller)),
    orientationChangeSignalConnected(false)
{
    // Connect related phone network indicators
    connect(phoneNetworkTypeIndicator, SIGNAL(networkAvailabilityChanged(bool)), phoneSignalStrengthIndicator, SLOT(setDisplay(bool)));

    // Connect notification signals
    connect(ScreenLockExtension::instance(), SIGNAL(notifierSinkActive(bool)), notifierIndicator, SLOT(setActive(bool)));

    // Put indicators into the layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(batteryIndicator);
    layout->addItem(phoneSignalStrengthIndicator);
    layout->addItem(phoneNetworkIndicator);
    layout->addItem(phoneNetworkTypeIndicator);
    layout->addStretch();
    layout->addItem(notifierIndicator);
    layout->addItem(transferStatusIndicator);
    layout->addItem(callForwardingIndicator);
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

void LockScreenStatusAreaView::setGeometry(const QRectF &rect)
{
    MWidgetView::setGeometry(rect);

    if (!orientationChangeSignalConnected && controller->sceneManager() != NULL) {
        // Update the status bar geometry property when the orientation change animation finishes.
        // This connection can not be made in the constructor because the scene manager does not exist at that point. In setGeometry() the manager definitely exists.
        connect(controller->sceneManager(), SIGNAL(orientationChangeFinished(M::Orientation)), this, SLOT(updateStatusBarGeometryProperty()));
        orientationChangeSignalConnected = true;

        // Do the initial update of the status bar geometry property
        updateStatusBarGeometryProperty();
    }
}

void LockScreenStatusAreaView::updateStatusBarGeometryProperty()
{
    if (controller->scene() != NULL) {
        Display *display = QX11Info::display();
        Atom atom = X11Wrapper::XInternAtom(display, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY", False);
        long data[4];

        foreach (QGraphicsView *view, controller->scene()->views()) {
            // Set the _MEEGOTOUCH_MSTATUSBAR_GEOMETRY property into each view (window) the StatusArea belongs to
            QRectF statusBarGeometry = controller->mapRectToScene(QRectF(QPointF(), controller->geometry().size()));
            if (statusBarGeometry != updatedStatusBarGeometry && statusBarGeometry.x() >= 0 && statusBarGeometry.y() >= 0) {
                data[0] = statusBarGeometry.x();
                data[1] = statusBarGeometry.y();
                data[2] = statusBarGeometry.width();
                data[3] = statusBarGeometry.height();
                X11Wrapper::XChangeProperty(display, view->winId(), atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)data, 4);

                updatedStatusBarGeometry = statusBarGeometry;
            }
        }
    }
}

M_REGISTER_VIEW_NEW(LockScreenStatusAreaView, StatusArea)
