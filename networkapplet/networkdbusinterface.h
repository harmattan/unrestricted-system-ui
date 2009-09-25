#ifndef NETWORKDBUSINTERFACE_H
#define NETWORKDBUSINTERFACE_H

#include <QObject>
#include <QStringList>

class QDBusInterface;

class NetworkDBusInterface : public QObject
{
    Q_OBJECT

public:
    NetworkDBusInterface();
    virtual ~NetworkDBusInterface();

    void phoneNetworkValueRequired();
    void enableRoamingValueRequired();
    void enableRoamingUpdatesValueRequired();
    void availableNetworksRequired();
    void setPhoneNetworkValue(bool value);
    void setEnableRoamingValue(bool value);
    void setEnableRoamingUpdatesValue(bool value);
    void setNetworkSelectionValue(const QString &value);

public slots:
    void setNetworkModeValue(const QString &value);

private slots:
    void valueSet();
    void DBusMessagingFailure();    

signals:
    void phoneNetworkValueReceived(bool);
    void enableRoamingUpdatesValueReceived(bool);
    void enableRoamingValueReceived(bool);
    void availableNetworksReceived(QStringList);

private:
    QDBusInterface *dbusIf;

    //test
    bool phoneNetworkValue;
    bool roamingEnabledValue;
    bool roamingUpdatesEnabledValue;

};

#endif // NETWORKDBUSINTERFACE_H
