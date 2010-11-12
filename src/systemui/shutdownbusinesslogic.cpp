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

extern sighandler_t originalSigIntHandler;

ShutdownBusinessLogic::ShutdownBusinessLogic (QObject *parent) :
    QObject (parent),
    m_Ui (0)
{
#ifdef HAVE_QMSYSTEM
    m_State = new MeeGo::QmSystemState (this);
    connect (
        m_State, 
        SIGNAL(systemStateChanged (MeeGo::QmSystemState::StateIndication)),
        this, 
        SLOT(systemStateChanged (MeeGo::QmSystemState::StateIndication)));
#endif
}

ShutdownBusinessLogic::~ShutdownBusinessLogic ()
{
    if (m_Ui) {
        m_Ui->deleteLater ();
        m_Ui = 0;
    }
}

void 
ShutdownBusinessLogic::showUI (
    QString  text1,
    QString  text2,
    int      timeout)
{
    if (m_Ui == 0) {
        m_Ui = new ShutdownUI;
        m_Ui->showWindow (text1, text2, timeout);
    }
}

#ifdef HAVE_QMSYSTEM
/*!
 * This function is called when the QmSystem reports a state change.
 */
void 
ShutdownBusinessLogic::systemStateChanged (
        MeeGo::QmSystemState::StateIndication what)
{
    switch (what) {
        case MeeGo::QmSystemState::Shutdown:
            // To avoid early quitting on shutdown...
            signal(SIGINT, originalSigIntHandler);
            showUI ();
            break;

        case MeeGo::QmSystemState::ThermalStateFatal:
            thermalShutdown ();
            break;

        case MeeGo::QmSystemState::ShutdownDeniedUSB:
            shutdownDeniedUSB ();
            break;

        case MeeGo::QmSystemState::BatteryStateEmpty:
            batteryShutdown ();
            break;

        default:
            break;
    }
}
#endif

void 
ShutdownBusinessLogic::thermalShutdown ()
{
    /*
     * Creating a notification.
     */
    MNotification notification (
           "x-nokia.battery.temperature",
           "", 
           //% "Temperature too high. Device shutting down."
           qtTrId ("qtn_shut_high_temp"));

    /*
     * Playing/publishing them.
     */
    notification.publish ();
}

void
ShutdownBusinessLogic::batteryShutdown ()
{
    /*
     * Creating a notification.
     */
    MNotification notification (
		   "x-nokia.battery.shutdown",
           "",
           //% "Battery empty. Device shutting down."
           qtTrId ("qtn_shut_batt_empty"));

    notification.publish ();
}

void 
ShutdownBusinessLogic::shutdownDeniedUSB ()
{
    /*
     * Creating the feedback.
     */
    MFeedback feedback("IDF_INFORMATION_SOUND");
    
    /*
     * Creating a notification.
     */
    MNotification notification (
           /* this has the usb-icon: */
		   MNotification::DeviceAddedEvent,
           "", 
           //% "USB cable plugged in. Unplug the USB cable to shutdown."
           qtTrId ("qtn_shut_unplug_usb"));

    /*
     * Playing/publishing them.
     */
    notification.publish ();
    feedback.play();
}

/******************************************************************************
 * Implementation for ShutdownBusinessLogicAdaptor.
 */
ShutdownBusinessLogicAdaptor::ShutdownBusinessLogicAdaptor (
    QObject                 *parent,
    ShutdownBusinessLogic   *logic) :
    QDBusAbstractAdaptor (parent),
    m_logic (logic)
{
}

void
ShutdownBusinessLogicAdaptor::showScreen (
    QString text1,
    QString text2,
    int     timeout)
{
    m_logic->showUI (text1, text2, timeout);
}

