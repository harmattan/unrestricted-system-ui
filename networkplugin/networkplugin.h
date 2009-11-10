#ifndef NETWORKPLUGIN_H
#define NETWORKPLUGIN_H

#include <DuiStatusIndicatorMenuPluginInterface>
#include <QObject>

class NetworkPlugin : public QObject, public DuiStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiStatusIndicatorMenuPluginInterface)

public:
    // Methods derived from DuiStatusIndicatorMenuPlugin
    virtual DuiWidget *constructWidget(DuiStatusIndicatorMenuInterface &statusIndicatorMenu);
};

#endif // NETWORKPLUGIN_H
