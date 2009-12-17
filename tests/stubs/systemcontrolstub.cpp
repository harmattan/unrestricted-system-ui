#include "systemcontrolstub.h"

namespace Cellular
{

SystemControl::SystemControl(QObject *parent) :
        QObject(parent)
{
}

SystemControl::~SystemControl()
{
}

bool SystemControl::power() const
{
    return true;
}

void SystemControl::setPower(bool value)
{
    Q_UNUSED(value);
}

}
