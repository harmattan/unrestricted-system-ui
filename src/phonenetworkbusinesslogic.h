#ifndef PHONENETWORKBUSINESSLOGIC_H
#define PHONENETWORKBUSINESSLOGIC_H

#include "notifier.h"
#include "systemuigconf.h"

#include <QObject>

class PhoneNetworkBusinessLogic : public QObject
{
public:
    PhoneNetworkBusinessLogic(SystemUIGConf *systemUIGConf);
    virtual ~PhoneNetworkBusinessLogic();

    void setNetworkMode(const QString &value);
    void setNetworkSelection(const QString &value);
    void toggleNetwork(bool toggle);
    void toggleRoaming(bool toggle);
    void toggleRoamingUpdates(bool toggle);
    void selectNetwork(QString value);
    QStringList networkModes();    
    QStringList availableNetworks();
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);

private: //attributes
    SystemUIGConf *systemUIGConf;

};

#endif // PHONENETWORKBUSINESSLOGIC_H
