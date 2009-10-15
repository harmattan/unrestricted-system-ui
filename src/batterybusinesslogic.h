#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include "notifier.h"
#include "systemuigconf.h"
//#include <qmsystem/qmbattery.h>
#include "batterystub.h"
//#include <qmsystem/qmdevicemode.h> //replaced with stub class
#include "devicemodestub.h"

#include <QObject>

class QTimer;


using namespace Maemo;

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT
public:
    BatteryBusinessLogic(SystemUIGConf *systemUIGConf);
    virtual ~BatteryBusinessLogic();

    void setPSMThreshold(const QString &threshold);
    void togglePSM(const QString &value);
    void togglePSMAuto(bool toggle);
    int batteryLevelValue();
    QString PSMValue();
    bool isBatteryCharging();
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);
    QStringList remainingTimeValues();
    QStringList PSMThresholdValues();

signals:
    void batteryCharging();
    void batteryNotCharging();
    void batteryLevelValueChanged(int);
    void PSMValueChanged(QString);
    void PSMAutoValueChanged(bool);

public slots:
    void initBattery();

private: //attributes    
    QmBattery *battery;
    QmDeviceMode *deviceMode;    
    Notifier *uiNotif;
    SystemUIGConf *systemUIGConf;
    QHash<QmBattery::Level, int> batteryLevels;

private: //methods
    void initSystemUIGConfKeys();
    void checkPSMThreshold(Maemo::QmBattery::Level level);

private slots:
    void batteryLevelChanged(Maemo::QmBattery::Level level);
    void batteryStatusChanged(Maemo::QmBattery::State state);
    void devicePSMStateChanged(Maemo::QmDeviceMode::PSMState PSMState);    
    void utiliseLED(bool activate, const QString &pattern);

};


#endif // BATTERYBUSINESSLOGIC_H

