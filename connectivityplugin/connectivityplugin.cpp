#include "connectivityplugin.h"
#include "connectivity.h"

Q_EXPORT_PLUGIN2(connectivity, ConnectivityPlugin);

DuiWidget* ConnectivityPlugin::constructWidget(DuiStatusIndicatorMenuInterface &statusIndicatorMenu)
{
    return new Connectivity(statusIndicatorMenu);
}
