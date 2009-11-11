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
    void currentOperatorValueRequired();
    void roamingValueRequired();
    void roamingUpdatesValueRequired();    
    void networkModeValuesRequired();    
    void networkSelectionValuesRequired();    
    void availableNetworksRequired();
    void networkIconValueRequired();
    void signalStrengthIconValueRequired();
    void setAutoNetworkSelection();
    void networkAppletClosing();

public slots:
    void setPhoneNetworkValue(bool value);
    void setRoamingValue(bool value);
    void setRoamingUpdatesValue(bool value);
    void setNetworkModeValue(const QString &value);
    void setNetworkSelectionValue(const QString &value);
    void setSelectedNetworkValue(const QString &value);    

private slots:
    void valueSet();
    void querySent();
    void DBusMessagingFailure();    

signals:
    void phoneNetworkValueReceived(bool);
    void currentOperatorValueReceived(QString);
    void roamingValueReceived(bool);
    void roamingUpdatesValueReceived(bool);    
    void networkModeValuesReceived(int, QStringList);
    void networkSelectionValuesReceived(int, int, QStringList);
    void availableNetworksReceived(int, QStringList, bool);
    void networkSelected(bool);
    void networkIconValueReceived(QString);
    void signalStrengthIconValueReceived(QString);

private:
    QDBusInterface *dbusIf;

};

#endif // NETWORKDBUSINTERFACE_H
