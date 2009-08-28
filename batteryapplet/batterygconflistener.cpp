#include "batterygconflistener.h"
#include <QDebug>

BatteryGConfListener::BatteryGConfListener()
{
}

BatteryGConfListener::~BatteryGConfListener()
{
}

void BatteryGConfListener::test(QString key, double val)
{    
    qDebug() << "\n\nkey: " << key << " value: " << val << "\n\n";
}
