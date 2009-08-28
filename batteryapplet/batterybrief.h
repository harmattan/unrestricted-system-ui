#ifndef BATTERYBRIEF_H
#define BATTERYBRIEF_H

#include "dcpbrief.h"

class BatteryBrief: public DcpBrief
{
    Q_OBJECT

public:
    virtual QString valueText() const;

};

#endif // BATTERYBRIEF
