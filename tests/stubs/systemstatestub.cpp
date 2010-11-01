#include "systemstatestub.h"

#include <QDebug>

namespace MeeGo {

QmSystemState::QmSystemState(QObject *parent)
             : QObject(parent)
{
}

QmSystemState::~QmSystemState()
{
}

bool QmSystemState::set(NextState nextState)
{
    qDebug() << "QmSystemState::set(" << nextState << ")";
    return true;
}

void QmSystemState::emitShutdown() {
    emit systemStateChanged(Shutdown);
}

void QmSystemState::emitThermalShutdown(QString thermalState)
{
    if (thermalState == "fatal") {
        emit systemStateChanged(ThermalStateFatal);
    }
}

void QmSystemState::emitBatteryShutdown()
{
    emit systemStateChanged(BatteryStateEmpty);
}

void QmSystemState::emitSaveData()
{
    emit systemStateChanged(SaveData);
}

void QmSystemState::emitShutdownDenied(QString reqType, QString reason)
{
    if (reason == "usb") {
        if (reqType == "shutdown") {
            emit systemStateChanged(ShutdownDeniedUSB);
        } else if (reqType == "reboot") {
            emit systemStateChanged(RebootDeniedUSB);
        }
    }
}

}
