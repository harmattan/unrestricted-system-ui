#include "offlineplugin.h"
#include "offline.h"

Q_EXPORT_PLUGIN2(offline, OfflinePlugin);

DuiWidget* OfflinePlugin::constructWidget(DuiStatusIndicatorMenuInterface& statusIndicatorMenu)
{
    return new Offline(statusIndicatorMenu);
}
