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
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "shutdownbusinesslogic.h"
#include "shutdownui.h"
#include "sysuid.h"

#include <qmsystem/qmsystemstate.h>

#include <MApplicationWindow>
#include <MApplication>
#include <MSceneManager>
#include <MNotification>
#include <MLocale>

#define DEBUG
#include "debug.h"

using namespace Maemo;

#ifndef UNIT_TEST
extern MApplication *exitPtr;
#else
MApplication *exitPtr;
#endif

ShutdownBusinessLogic::ShutdownBusinessLogic (QObject *parent) :
    QObject (parent),
    m_Ui (0),
    m_State (new QmSystemState (this))
{
    connect (
        m_State, 
        SIGNAL(systemStateChanged (Maemo::QmSystemState::StateIndication)),
        this, 
        SLOT(systemStateChanged (Maemo::QmSystemState::StateIndication)));
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
    if (m_Ui == 0)
        m_Ui = new ShutdownUI;

    m_Ui->showWindow (text1, text2, timeout);
}

/*!
 * This function is called when the QmSystem reports a state change.
 */
void 
ShutdownBusinessLogic::systemStateChanged (
        QmSystemState::StateIndication what)
{
    switch (what) {
        case QmSystemState::Shutdown:
            SYS_DEBUG ("QmSystemState::Shutdown");
            // To avoid early quitting on shutdown...
            exitPtr = 0;
            showUI ();
            break;

        case QmSystemState::ThermalStateFatal:
            SYS_DEBUG ("QmSystemState::ThermalStateFatal");
            thermalShutdown ();
            break;
            
        case QmSystemState::ShutdownDeniedUSB:
            SYS_DEBUG ("QmSystemState::ShutdownDeniedUSB");
            shutdownDeniedUSB ();
            break;

        case QmSystemState::BatteryStateEmpty:
            SYS_DEBUG ("QmSystemState::BatteryStateEmpty");
            batteryShutdown ();
            break;

        default:
            SYS_WARNING ("Unknown state");
            break;
    }
}

void 
ShutdownBusinessLogic::thermalShutdown ()
{
    SYS_DEBUG ("");
    //% "Temperature too high. Device shutting down."
    MNotification(MNotification::DeviceEvent, "", qtTrId ("qtn_shut_high_temp")).publish();

    /* TODO: do we need to call showUI here?
     * UI spec says: Ten seconds before the shutdown takes place, thermal 
     * shutdown notification is displayed accompanying ‘System alert’ sound.
     */
}

void
ShutdownBusinessLogic::batteryShutdown ()
{
    //% "Battery empty. Device shutting down."
    MNotification (MNotification::DeviceEvent, "", qtTrId ("qtn_shut_batt_empty")).publish();
}

void 
ShutdownBusinessLogic::shutdownDeniedUSB ()
{
    //% "USB cable plugged in. Unplug the USB cable to shutdown."
    MNotification(MNotification::DeviceEvent, "", qtTrId ("qtn_shut_unplug_usb")).publish();
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
    SYS_DEBUG ("text1 = %s, text2 = %s, timeout = %d",
               SYS_STR (text1), SYS_STR (text2), timeout);

    m_logic->showUI (text1, text2, timeout);
}

