#ifndef CONNECTIVITYPLUGIN_H
#define CONNECTIVITYPLUGIN_H

#include <DuiStatusIndicatorMenuPluginInterface>
#include <QObject>

class ConnectivityPlugin : public QObject, public DuiStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiStatusIndicatorMenuPluginInterface)

public:
    // Methods derived from DuiStatusIndicatorMenuPlugin
    virtual DuiWidget *constructWidget(DuiStatusIndicatorMenuInterface &statusIndicatorMenu);
};

#endif // CONNECTIVITYPLUGIN_H
