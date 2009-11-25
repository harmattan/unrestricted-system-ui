#include "networksignalstrengthicon.h"

#include <QDebug>

NetworkSignalStrengthIcon::NetworkSignalStrengthIcon(QGraphicsItem *parent) :
        DuiImage(parent),
        signalStrength(NULL)
{
    signalStrength = new SignalStrength();
    connect(signalStrength, SIGNAL(levelChanged(int, int)), this, SLOT(signalStrengthChanged(int)));
    setImage(mapSignalStrengthToIcon(signalStrength->bars()));
}

NetworkSignalStrengthIcon::~NetworkSignalStrengthIcon()
{
    delete signalStrength;
    signalStrength = NULL;
}

void NetworkSignalStrengthIcon::signalStrengthChanged(int bars)
{
    setImage(mapSignalStrengthToIcon(bars));
}

QString NetworkSignalStrengthIcon::mapSignalStrengthToIcon(int bars)
{    
    QString icon;
    if(bars % 20 == 0 && bars > 0)
        icon = QString("icon-s-network-%1").arg(bars);
    else
        icon = QString("icon-s-network-%1").arg(bars/20*20 + 20);
    return icon;
}

