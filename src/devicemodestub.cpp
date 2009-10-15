#include "devicemodestub.h"

namespace Maemo {

QmDeviceMode::QmDeviceMode(QObject *parent) :
        QObject(parent),
        state(QmDeviceMode::PSMStateOff)
{
}

QmDeviceMode::~QmDeviceMode()
{
}


QmDeviceMode::PSMState QmDeviceMode::getPSMState() const
{
    return state;
}

bool QmDeviceMode::setPSMState(QmDeviceMode::PSMState state){
    this->state = state;
    emit devicePSMStateChanged(state);
}

}
