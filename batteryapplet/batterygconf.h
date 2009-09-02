#ifndef BATTERYGCONF_H
#define BATTERYGCONF_H

#include <QHash>
#include <QObject>
#include <QVariant>

class DuiGConfItem;

class BatteryGConf : public QObject
{
    Q_OBJECT
public:

    enum GConfKey {
        PSMToggleKey,
        PSMDisabledKey,
        PSMThresholdKey,        
        PSMThresholdValuesKey,
        RemainingTalkTimeKey,
        RemainingStandByTimeKey,
        BatteryLevelKey,
        ChargingKey,
        BatterySystemSettingInUseKey
    };

    BatteryGConf();
    virtual ~BatteryGConf();

    void setValue(BatteryGConf::GConfKey, QVariant value);
    QVariant value(BatteryGConf::GConfKey);

signals:    
    void valueChanged(BatteryGConf::GConfKey key, QVariant value);

private slots:
    void keyValueChanged();

private: //methods
    QString mapGConfKey(BatteryGConf::GConfKey key);

private: //attributes
    QHash <BatteryGConf::GConfKey, DuiGConfItem *> duiGConfItems;

};


#endif // BATTERYGCONF_H
