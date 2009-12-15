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
#include <SSC>
#endif
#include "networktechnology.h"
#include "systemuigconf.h" //temp

using namespace Cellular;

class NetworkBusinessLogic : public QObject
{
    Q_OBJECT

public:
    NetworkBusinessLogic(QObject* parent = 0);
    virtual ~NetworkBusinessLogic();

    bool networkEnabled(); //temp
    int roamingState(); //temp
    bool roamingUpdatesState(); //temp
    QStringList networkModes();
    QString selectedNetworkMode();
    QStringList networkSelectionValues();
    QString selectedNetworkSelectionValue();
    QString defaultNetworkSelectionValue();
    QString networkIcon();
    void changeRoamingState(int state);    
    bool manualSelectionRequired();
    QString currentOperator();    

public slots:
    void toggleNetwork(bool toggle);
    void setNetworkMode(const QString &value);
    void setNetworkSelectionValue(const QString &value);
    void selectOperator(int index);
    void changeRoamingUpdatesState(bool state);

private slots:
    void availableOperatorsReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason);
    void selectOperatorCompleted(bool success, const QString &reason);
    void networkToggleCompleted(SSC::SSCError error);
    void technologyChanged(NetworkTechnology::Technology technology);

    void tempSlot(SystemUIGConf::GConfKey,QVariant); //temp

signals:
    void availableNetworkOperators(int, QStringList);
    void autoSelectNetworkOperator();
    void networkIconChanged(QString);
    void networkSelectionValuesAvailable(int, int, QStringList);    
    void networkSelected(bool);
    void roamingStateChanged(int);
    void roamingUpdatesStateChanged(bool);
    void networkOperatorChanged(QString);    
    void showNotification(QString);
    void operatorSelectionFailed();

    void networkStateChanged(bool); //temp

private: //methods    
    void queryAvailableOperators();
    QString mapTechnologyToIcon(NetworkTechnology::Technology technology);    

private: //attributes
    NetworkRegistration *networkRegistration;
    RadioAccess *radioAccess;
    NetworkOperator *networkOperator;
    SSC *ssc;
    NetworkTechnology *technology;
    QHash<RadioAccess::Mode, QString> modes;
    QHash<NetworkRegistration::Mode, QString> selectionValues;
    QMultiMap<QString, QStringList> operators;    

    SystemUIGConf *systemUIGConf; //temp

#ifdef UNIT_TEST
   friend class Ut_NetworkBusinessLogic;
#endif // UNIT_TEST

};

#endif // NETWORKBUSINESSLOGIC_H
