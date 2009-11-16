#ifndef OFFLINEPLUGIN_H
#define OFFLINEPLUGIN_H

#include <DuiStatusIndicatorMenuPluginInterface>
#include <QObject>

class OfflinePlugin : public QObject, public DuiStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiStatusIndicatorMenuPluginInterface)

public:
    virtual DuiWidget *constructWidget(DuiStatusIndicatorMenuInterface& statusIndicatorMenu);
};

#endif // OFFLINEPLUGIN_H
