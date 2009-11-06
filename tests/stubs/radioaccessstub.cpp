#include "radioaccessstub.h"

namespace Cellular
{

static RadioAccess::Mode currentMode = RadioAccess::DualMode;

RadioAccess::RadioAccess(QObject *parent) :
        QObject(parent)
{
}

RadioAccess::~RadioAccess()
{
}

RadioAccess::Mode RadioAccess::mode() const
{
    return currentMode;
}

void RadioAccess::setMode(RadioAccess::Mode mode)
{
    currentMode = mode;
    emit modeChanged(mode);
}

}
