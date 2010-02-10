/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "shutdownbusinesslogic.h"
#include "shutdownui.h"

#include <qmsystem/qmsystemstate.h>

#include <DuiNotification>
#include <DuiLocale>

//#include <QDebug>

#define DEBUG
#include "../debug.h"

using namespace Maemo;

#ifdef TEST_SHUTDOWN_LOGIC
#  include <QTimer>
#endif

ShutdownBusinessLogic::ShutdownBusinessLogic (
        QObject *parent) :
    QObject (parent),
    m_Ui (NULL),
    m_State (new QmSystemState (this))
{
    connect (
        m_State, 
        SIGNAL(systemStateChanged (Maemo::QmSystemState::StateIndication)),
        this, 
        SLOT(systemStateChanged (Maemo::QmSystemState::StateIndication)));

    #ifdef TEST_SHUTDOWN_LOGIC
    SYS_DEBUG ("Register test timeout.");
    QTimer::singleShot (5000, this, SLOT(testTimeoutSlot()));
    #endif
}

ShutdownBusinessLogic::~ShutdownBusinessLogic ()
{
    if (m_Ui) {
        m_Ui->deleteLater();
        m_Ui = NULL;
    }
}


void 
ShutdownBusinessLogic::showUI ()
{
    ShutdownUI::showWindow();

    if (!m_Ui) {
        m_Ui = new ShutdownUI;
    }
    m_Ui->appear();
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
    DuiNotification("", "", qtTrId ("qtn_shut_high_temp"));

    /* TODO: do we need to call showUI here?
     * UI spec says: Ten seconds before the shutdown takes place, thermal 
     * shutdown notification is displayed accompanying ‘System alert’ sound.
     */
}

void
ShutdownBusinessLogic::batteryShutdown ()
{
    //% "Battery empty. Device shutting down."
    DuiNotification("", "", qtTrId ("qtn_shut_batt_empty"));
}

void 
ShutdownBusinessLogic::shutdownDeniedUSB ()
{
    //% "USB cable plugged in. Unplug the USB cable to shutdown."
    DuiNotification("", "", qtTrId ("qtn_shut_unplug_usb"));
}

#ifdef TEST_SHUTDOWN_LOGIC
void 
ShutdownBusinessLogic::testTimeoutSlot ()
{
    SYS_DEBUG ("");
    systemStateChanged (QmSystemState::ShutdownDeniedUSB);
}
#endif
