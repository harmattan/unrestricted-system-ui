#include "networkplugin.h"
#include "network.h"

Q_EXPORT_PLUGIN2(network, NetworkPlugin);

DuiWidget* NetworkPlugin::constructWidget(DuiStatusIndicatorMenuInterface &statusIndicatorMenu)
{
    return new Network(statusIndicatorMenu);
}
