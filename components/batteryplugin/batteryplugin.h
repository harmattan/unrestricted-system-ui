#ifndef BATTERYPLUGIN_H
#define BATTERYPLUGIN_H

#include <DuiStatusIndicatorMenuPluginInterface>
#include <QObject>

class BatteryPlugin : public QObject, public DuiStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiStatusIndicatorMenuPluginInterface)

public:
    // Methods derived from DuiStatusIndicatorMenuPlugin
    virtual DuiWidget *constructWidget (
        DuiStatusIndicatorMenuInterface &statusIndicatorMenu);
};

#endif // BATTERYPLUGIN_H
