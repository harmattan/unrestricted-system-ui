#include "radioaccessstub.h"
#include <QDebug>

namespace Cellular
{

RadioAccess::RadioAccess(QObject *parent) :
        QObject(parent),
        currentMode(RadioAccess::GSMMode),
        currentTechnology(RadioAccess::GSM),
        flags(RadioAccess::Idle)
{
}

RadioAccess::~RadioAccess()
{
}

RadioAccess::Mode RadioAccess::mode() const
{
    return currentMode;
}

RadioAccess::Technology RadioAccess::technology() const
{    
    return currentTechnology;
}

RadioAccess::Flags RadioAccess::state() const
{
    return flags;
}

void RadioAccess::setMode(RadioAccess::Mode mode)
{
    currentMode = mode;
    emit modeChanged((int)mode);   
}

void RadioAccess::setTechnology(RadioAccess::Technology technology)
{
    currentTechnology = technology;
    emit technologyChanged((int)technology);
}

void RadioAccess::setState(RadioAccess::State state)
{
    flags = state;
    emit statusChanged(flags);
}

}
