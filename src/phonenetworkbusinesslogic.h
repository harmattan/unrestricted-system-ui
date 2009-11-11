#ifndef PHONENETWORKBUSINESSLOGIC_H
#define PHONENETWORKBUSINESSLOGIC_H

#include "notifier.h"
#include "systemuigconf.h"

#include <QObject>
#include <QStringList>
#include <QTimer>

#include <NetworkRegistration>
#include <RadioAccess>
#include <NetworkCell>
#include <NetworkOperator>
#include <SignalStrength>

using namespace Cellular;

class PhoneNetworkTechnology : public QObject
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

    PhoneNetworkTechnology(RadioAccess *radioAccess, QObject *parent = 0);
    ~PhoneNetworkTechnology();

    PhoneNetworkTechnology::Technology currentTechnology();

private slots:
    void checkTechnology();

signals:
    void technologyChanged(PhoneNetworkTechnology::Technology technology);

private: //methods
    void updateTechnology();

private: //attributes
    RadioAccess *radioAccess;
    NetworkCell *networkCell;
    PhoneNetworkTechnology::Technology technology;
    QTimer *timer;

};


class PhoneNetworkBusinessLogic : public QObject
{
    Q_OBJECT

public:
    PhoneNetworkBusinessLogic(SystemUIGConf* systemUIGConf, QObject* parent = 0);
    virtual ~PhoneNetworkBusinessLogic();

    bool phoneNetworkEnabled();
    bool roamingEnabled();
    bool roamingUpdatesEnabled();    
    void setNetworkMode(const QString &value);
    void setNetworkSelection(const QString &value);
    void selectNetwork(const QString &value);
    void toggleNetwork(bool toggle);
    void toggleRoaming(bool toggle);
    void toggleRoamingUpdates(bool toggle);
    void queryNetworkModes();
    void queryNetworkSelectionValues();
    QString networkIcon();
    QString currentOperator();
    QString signalStrengthIcon();
    void networkAppletClosing();    
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);    

private slots:
    void selectNetworkCompleted(bool success, const QString &reason);
    void availableNetworksReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason);
    void technologyChanged(PhoneNetworkTechnology::Technology technology);
    void signalStrengthChanged(int bars);

signals:
    void networkModeValuesAvailable(int, QStringList);
    void networkSelectionValuesAvailable(int, int, QStringList);
    void availableNetworksAvailable(int, QStringList, bool);
    void networkSelected(bool);
    void roamingUpdatesValueChanged(bool);
    void networkIconChanged(QString);
    void networkOperatorChanged(QString);
    void signalStrengthIconChanged(QString);
    void showNotification(QString);

private: //methods
    void queryAvailableNetworks();
    QString mapTechnologyToIcon(PhoneNetworkTechnology::Technology technology);
    QString mapSignalStrengthToIcon(int bars);

private: //attributes
    SystemUIGConf *systemUIGConf;
    NetworkRegistration *networkRegistration;
    RadioAccess *radioAccess;
    NetworkOperator *networkOperator;
    SignalStrength *signalStrength;
    PhoneNetworkTechnology *technology;
    QHash<RadioAccess::Mode, QString> networkModes;
    QHash<NetworkRegistration::Mode, QString> networkSelectionValues;
    QHash<QString, QStringList> operators;    

    bool temp; //for temp usage

};

#endif // PHONENETWORKBUSINESSLOGIC_H
