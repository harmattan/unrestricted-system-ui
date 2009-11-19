#include "shutdownbusinesslogic.h"
#include "shutdownui.h"

#include <qmsystem/qmsystemstate.h>

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

void ShutdownBusinessLogic::systemStateChanged(QmSystemState::StateIndication what)
{
    switch (what) {
    case QmSystemState::NormalShutdown:
        normalShutdown();
        break;
    case QmSystemState::ThermalShutdown:
        thermalShutdown();
        break;
/* These are not in the API yet

    case QmSystemState::ShutdownDeniedUSB:
        shutdownDeniedUSB();
        break;
    case QmSystemState::
        batteryShutdown();
        break;
*/
    default:
        break;
    }
}

void ShutdownBusinessLogic::normalShutdown()
{
    if (!ui) {
        ui = new ShutdownUI;
    }
    ui->appear();
}

void ShutdownBusinessLogic::thermalShutdown()
{
    const QString msg = trid("qtn_shut_high_temp", "Temperature too high. Device shutting down.");
    emit showNotification(msg, NotificationType::warning);
}

void ShutdownBusinessLogic::batteryShutdown()
{
    const QString msg = trid("qtn_shut_batt_empty", "Battery empty. Device shutting down.");
    emit showNotification(msg, NotificationType::warning);
}

void ShutdownBusinessLogic::shutdownDeniedUSB()
{
    const QString msg = trid("qtn_shut_unplug_usb", "USB cable plugged in. Unplug the USB cable to shutdown.");
    emit showNotification(msg, NotificationType::warning);
}
