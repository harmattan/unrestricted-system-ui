#ifndef NETWORKBUSINESSLOGIC_H
#define NETWORKBUSINESSLOGIC_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QMultiMap>

#ifdef UNIT_TEST
#include "networkregistrationstub.h"
#include "radioaccessstub.h"
#include "networkoperatorstub.h"
#else
#include <NetworkRegistration>
#include <RadioAccess>
#include <NetworkOperator>
#endif
#include "networktechnology.h"

using namespace Cellular;

class NetworkBusinessLogic : public QObject
{
    Q_OBJECT

public:
    NetworkBusinessLogic(QObject* parent = 0);
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

public slots:
    void toggleNetwork(bool toggle);
    void setNetworkMode(const QString &value);
    void setNetworkSelectionValue(const QString &value);
    void selectOperator(int index);

private slots:
    void availableOperatorsReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason);
    void selectOperatorCompleted(bool success, const QString &reason);
    void technologyChanged(NetworkTechnology::Technology technology);

signals:
    void availableNetworkOperators(int, QStringList, bool);
    void networkIconChanged(QString);
    void networkSelectionValuesAvailable(int, int, QStringList);    
    void networkSelected(bool);
    void roamingUpdatesValueChanged(bool);    
    void networkOperatorChanged(QString);    
    void showNotification(QString);

private: //methods    
    void queryAvailableOperators();
    QString mapTechnologyToIcon(NetworkTechnology::Technology technology);    

private: //attributes
    NetworkRegistration *networkRegistration;
    RadioAccess *radioAccess;
    NetworkOperator *networkOperator;    
    NetworkTechnology *technology;
    QHash<RadioAccess::Mode, QString> modes;
    QHash<NetworkRegistration::Mode, QString> selectionValues;
    QMultiMap<QString, QStringList> operators;    

    bool tempNetworkToggle; //for temp usage
    bool tempRoamingToggle; //for temp usage
    bool tempRoamingUpdatesToggle; //for temp usage

#ifdef UNIT_TEST
   friend class Ut_NetworkBusinessLogic;
#endif // UNIT_TEST

};

#endif // NETWORKBUSINESSLOGIC_H
