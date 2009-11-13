#ifndef NETWORKBUSINESSLOGIC_H
#define NETWORKBUSINESSLOGIC_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QMultiMap>
#include <QTimer>

#include <NetworkRegistration>
#include <RadioAccess>
#include <NetworkCell>
#include <NetworkOperator>
#include <SignalStrength>

class NetworkDBusInterface;

using namespace Cellular;

class NetworkTechnology : public QObject
{
    Q_OBJECT

public:

    enum Technology {
        None = 0,
        TwoG,
        TwoPointFiveG,
        ThreeG,
        ThreePointFiveG
    };

    NetworkTechnology(RadioAccess *radioAccess, QObject *parent = 0);
    ~NetworkTechnology();

    NetworkTechnology::Technology currentTechnology();

private slots:
    void checkTechnology();

signals:
    void technologyChanged(NetworkTechnology::Technology technology);

private: //methods
    void updateTechnology();

private: //attributes
    RadioAccess *radioAccess;
    NetworkCell *networkCell;
    NetworkTechnology::Technology technology;
    QTimer *timer;

};


class NetworkBusinessLogic : public QObject
{
    Q_OBJECT

public:
    NetworkBusinessLogic(QObject* parent = 0, bool monitorSignalStrength = false);
    virtual ~NetworkBusinessLogic();

    bool networkEnabled();
    bool roamingEnabled();
    bool roamingUpdatesEnabled();
    QStringList networkModes();
    QString selectedNetworkMode();
    QStringList networkSelectionValues();
    QString selectedNetworkSelectionValue();
    QString defaultNetworkSelectionValue();
    QString networkIcon();
    void toggleRoaming(bool toggle);
    void toggleRoamingUpdates(bool toggle);
    bool manualSelectionRequired();
    QString currentOperator();
    QString signalStrengthIcon();    

public slots:
    void toggleNetwork(bool toggle);
    void setNetworkMode(const QString &value);
    void setNetworkSelectionValue(const QString &value);
    void selectOperator(int index);

private slots:
    void availableOperatorsReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason);
    void selectOperatorCompleted(bool success, const QString &reason);
    void technologyChanged(NetworkTechnology::Technology technology);
    void signalStrengthChanged(int bars);

signals:
    void availableNetworkOperators(int, QStringList, bool);
    void networkIconChanged(QString);

    void networkSelectionValuesAvailable(int, int, QStringList);    
    void networkSelected(bool);
    void roamingUpdatesValueChanged(bool);    
    void networkOperatorChanged(QString);
    void signalStrengthIconChanged(QString);
    void showNotification(QString);

private: //methods
    void queryAvailableOperators();
    QString mapTechnologyToIcon(NetworkTechnology::Technology technology);
    QString mapSignalStrengthToIcon(int bars);

private: //attributes
    NetworkRegistration *networkRegistration;
    RadioAccess *radioAccess;
    NetworkOperator *networkOperator;
    SignalStrength *signalStrength;
    NetworkTechnology *technology;
    QHash<RadioAccess::Mode, QString> modes;
    QHash<NetworkRegistration::Mode, QString> selectionValues;
    QMultiMap<QString, QStringList> operators;    

    bool tempNetworkToggle; //for temp usage
    bool tempRoamingToggle; //for temp usage
    bool tempRoamingUpdatesToggle; //for temp usage

};

#endif // NETWORKBUSINESSLOGIC_H
