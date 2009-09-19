#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include "notifier.h"
#include "systemuigconf.h"
#include <qmsystem/qmbattery.h>
#include <qmsystem/qmdevicemode.h>

#include <QObject>

class QTimer;


using namespace Maemo;

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT
public:
    BatteryBusinessLogic(SystemUIGConf *systemUIGConf);
    virtual ~BatteryBusinessLogic();

    void togglePSMDisabled(bool disabled);
    void setPSMThreshold(const QString &threshold);
    void togglePSM(bool toggle);    
    int batteryLevelValue();
    bool batteryChargingState();
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);
    QStringList remainingTimeValues();
    QStringList PSMThresholdValues();

signals:
    void batteryCharging();
    void batteryNotCharging();
    void batteryLevelValueChanged(int level);
    void PSMToggleValueChanged(bool toggle);

public slots:
    void initBattery();

private: //attributes    
    QmBattery *battery;
    QmDeviceMode *deviceMode;    
    Notifier *uiNotif;
    SystemUIGConf *systemUIGConf;
    QHash<QmBattery::Level, QString> batteryLevels;

private: //methods
    void initSystemUIGConfKeys();
    void checkPSMThreshold(Maemo::QmBattery::Level level);    

private slots:
    void batteryLevelChanged(Maemo::QmBattery::Level level);
    void batteryStateChanged(Maemo::QmBattery::State state);    
    void activatePSM();
    void utiliseLED(bool activate, const QString &pattern);

};


#endif // BATTERYBUSINESSLOGIC_H

