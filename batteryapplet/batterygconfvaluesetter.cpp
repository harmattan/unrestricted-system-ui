#include "batterygconfvaluesetter.h"

#include <DuiGConfItem>

BatteryGConfValueSetter::BatteryGConfValueSetter(DuiGConfItem *item, QVariant value)
{
    this->item = item;
    this->value = value;
}

BatteryGConfValueSetter::~BatteryGConfValueSetter()
{
}

void BatteryGConfValueSetter::run()
{
    item->set(value);
}
