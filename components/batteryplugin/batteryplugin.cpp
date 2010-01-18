#include "batteryplugin.h"
#include "battery.h"

Q_EXPORT_PLUGIN2(battery, BatteryPlugin);

DuiWidget*
BatteryPlugin::constructWidget (
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu)
{
    return new Battery (statusIndicatorMenu);
}
