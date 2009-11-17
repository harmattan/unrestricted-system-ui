#include "signalstrengthstub.h"

#include <QDebug>

namespace Cellular {

SignalStrength::SignalStrength(QObject *parent) :
        QObject(parent),       
        currentBars(50),
        dummy(0)
{    
}

SignalStrength::~SignalStrength()
{
}

int SignalStrength::bars() const
{
    return currentBars;
}

void SignalStrength::setBars(int bars)
{
    currentBars = bars;   
    emit levelChanged(currentBars, dummy);
}

}
