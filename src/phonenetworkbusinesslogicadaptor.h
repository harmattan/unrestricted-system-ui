#ifndef PHONENETWORKBUSINESSLOGICADAPTOR_H
#define PHONENETWORKBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

#include <NetworkRegistration>

class PhoneNetworkBusinessLogic;

using namespace Cellular;

class PhoneNetworkBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.network")

public:
    PhoneNetworkBusinessLogicAdaptor(QObject *obj, PhoneNetworkBusinessLogic *networkLogic);

public slots:
    Q_NOREPLY void setPhoneNetworkValue(bool value);
    Q_NOREPLY void setRoamingValue(bool value);
    Q_NOREPLY void setRoamingUpdatesValue(bool value);
    Q_NOREPLY void setNetworkModeValue(const QString &value);
    Q_NOREPLY void setNetworkSelectionValue(const QString &value);
    Q_NOREPLY void setSelectedNetworkValue(const QString &value);
    Q_NOREPLY void networkModeValues();
    Q_NOREPLY void networkSelectionValues();    
    bool phoneNetworkValue();
    bool roamingValue();
    bool roamingUpdatesValue();

signals:
    void networkModeValuesReceived(int, QStringList);
    void networkSelectionValuesReceived(int, QStringList);
    void availableNetworksReceived(int, QStringList);    

private:
    PhoneNetworkBusinessLogic *networkLogic;    

};

#endif // PHONENETWORKBUSINESSLOGICADAPTOR_H
