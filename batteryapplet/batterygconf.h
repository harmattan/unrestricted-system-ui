#ifndef BATTERYGCONF_H
#define BATTERYGCONF_H

#include <QList>
#include <QHash>
#include <QObject>

class DuiConf;

class BatteryGConf
{

public:

    enum GConfKey {
        PSMToggleKey,
        PSMDisabledKey,
        PSMThresholdKey,
        PSMThresholdValueCountKey,
        PSMThresholdValueNKey,
        RemainingTalkTimeKey,
        RemainingStandByTimeKey,
        BatteryLevelKey,
        ChargingKey

        //PSMThersholdValues are set only once and won't be changed
    };

    BatteryGConf(QObject *listener, QHash<BatteryGConf::GConfKey, QString> &keysAndSlots);
    virtual ~BatteryGConf();

    void setPSMToggle(bool toggle);
    void setPSMDisabled(bool disabled);
    void setPSMThreshold(int threshold);
    void setPSMThresholdValues(QList<int> values);
    void setRemainingTalkTime(int time);
    void setRemainingStandByTime(int time);
    void setBatteryLevel(int level);
    void setCharging(bool charging);    

    bool PSMToggle();
    bool PSMDisabled();
    int PSMThreshold();
    QList<int> PSMThresholdValues();
    int remainingTalkTime();
    int remainingStandByTime();
    int batteryLevel();
    bool charging();

private: //methods
    QString mapGConfKey(BatteryGConf::GConfKey key);

private: //attributes
    DuiConf *duiConf;    

};


#endif // BATTERYGCONF_H
