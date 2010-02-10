#include "volumebarplugin.h"
#include "volumebar.h"

Q_EXPORT_PLUGIN2(volumebar, VolumeBarPlugin);

DuiWidget*
VolumeBarPlugin::constructWidget (
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu)
{
    return new VolumeBar (statusIndicatorMenu);
}
