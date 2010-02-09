#ifndef VOLUMEBARPLUGIN_H
#define VOLUMEBARPLUGIN_H

#include <DuiStatusIndicatorMenuPluginInterface>
#include <QObject>

class VolumeBarPlugin : public QObject, public DuiStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiStatusIndicatorMenuPluginInterface)

public:
    DuiWidget *constructWidget (
        DuiStatusIndicatorMenuInterface &statusIndicatorMenu);
};


#endif

