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
#include "shutdownbusinesslogic.h"
#include "shutdownui.h"
#include "sysuid.h"
#include <signal.h>

#include <MApplication>
#include <MFeedback>
#include <MSceneManager>
#include <MNotification>
#include <MLocale>

extern void restoreSignalHandlers();

ShutdownBusinessLogic::ShutdownBusinessLogic(QObject *parent) :
    QObject(parent),
    shutdownUi(NULL)
{
#ifdef HAVE_QMSYSTEM
    m_State = new MeeGo::QmSystemState(this);
    connect(m_State, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)),
        this, SLOT(systemStateChanged(MeeGo::QmSystemState::StateIndication)));
#endif
}

ShutdownBusinessLogic::~ShutdownBusinessLogic()
{
    delete shutdownUi;
}

void  ShutdownBusinessLogic::showUI(QString text1, QString text2, int timeout)
{
    if (shutdownUi == NULL) {
        shutdownUi = new ShutdownUI;
    }

    shutdownUi->showWindow(text1, text2, timeout);
}

#ifdef HAVE_QMSYSTEM

void ShutdownBusinessLogic::systemStateChanged(MeeGo::QmSystemState::StateIndication what)
{
    switch (what) {
        case MeeGo::QmSystemState::Shutdown:
            // To avoid early quitting on shutdown
            restoreSignalHandlers();
            showUI();
            break;

        case MeeGo::QmSystemState::ThermalStateFatal:
            thermalShutdown();
            break;

        case MeeGo::QmSystemState::ShutdownDeniedUSB:
            shutdownDeniedUSB();
            break;

        case MeeGo::QmSystemState::BatteryStateEmpty:
            batteryShutdown();
            break;

        default:
            break;
    }
}
#endif

void ShutdownBusinessLogic::createAndPublishNotification(const QString &type, const QString &summary, const QString &body)
{
    MNotification notification(type, summary, body);
    notification.publish();
}

void ShutdownBusinessLogic::thermalShutdown()
{
    //% "Temperature too high. Device shutting down."
    createAndPublishNotification("x-nokia.battery.temperature","", qtTrId("qtn_shut_high_temp"));
}

void ShutdownBusinessLogic::batteryShutdown()
{
    //% "Battery empty. Device shutting down."
    createAndPublishNotification("x-nokia.battery.shutdown", "", qtTrId("qtn_shut_batt_empty"));
}

void ShutdownBusinessLogic::shutdownDeniedUSB()
{
    //% "USB cable plugged in. Unplug the USB cable to shutdown."
    createAndPublishNotification(MNotification::DeviceAddedEvent, "", qtTrId("qtn_shut_unplug_usb"));
}

ShutdownBusinessLogicAdaptor::ShutdownBusinessLogicAdaptor(QObject *parent, ShutdownBusinessLogic *logic) :
    QDBusAbstractAdaptor(parent),
    shutdownBusinessLogic(logic)
{
}

void ShutdownBusinessLogicAdaptor::showScreen(QString text1, QString text2, int timeout)
{
    shutdownBusinessLogic->showUI(text1, text2, timeout);
}
