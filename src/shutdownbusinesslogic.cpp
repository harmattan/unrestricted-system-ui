#include "shutdownbusinesslogic.h"
#include "shutdownui.h"

#include <qmsystem/qmsystemstate.h>

#include <DuiNotification>
#include <DuiLocale>

#include <QDebug>

using namespace Maemo;

ShutdownBusinessLogic::ShutdownBusinessLogic(QObject* parent) :
        QObject(parent),
        ui(NULL),
        state(NULL)
{
    state = new QmSystemState(this);
    connect(state, SIGNAL(systemStateChanged(Maemo::QmSystemState::StateIndication)),
            this, SLOT(systemStateChanged(Maemo::QmSystemState::StateIndication)));
}

ShutdownBusinessLogic::~ShutdownBusinessLogic()
{
    if (ui) {
        ui->deleteLater();
        ui = NULL;
    }
}

void ShutdownBusinessLogic::showUI()
{
    ShutdownUI::showWindow();

    if (!ui) {
        ui = new ShutdownUI;
    }
    ui->appear();
}

void ShutdownBusinessLogic::systemStateChanged(QmSystemState::StateIndication what)
{
    switch (what) {
    case QmSystemState::Shutdown:
        showUI();
        break;
    case QmSystemState::ThermalStateFatal:
        thermalShutdown();
        break;
    case QmSystemState::ShutdownDeniedUSB:
        shutdownDeniedUSB();
        break;
    case QmSystemState::BatteryStateEmpty:
        batteryShutdown();
        break;
    default:
        break;
    }
}

void ShutdownBusinessLogic::thermalShutdown()
{
    DuiNotification("", "", trid("qtn_shut_high_temp", "Temperature too high. Device shutting down."));

    /* TODO: do we need to call showUI here?
    UI spec says: Ten seconds before the shutdown takes place, thermal shutdown notification is displayed accompanying ‘System alert’ sound.
    */
}

void ShutdownBusinessLogic::batteryShutdown()
{
    DuiNotification("", "", trid("qtn_shut_batt_empty", "Battery empty. Device shutting down."));
}

void ShutdownBusinessLogic::shutdownDeniedUSB()
{
    DuiNotification("", "", trid("qtn_shut_unplug_usb", "USB cable plugged in. Unplug the USB cable to shutdown."));
}
