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
        RemainingTimesKey,
        BatteryLevelKey,
        ChargingKey
   };

    BatteryGConf();
    virtual ~BatteryGConf();

    void setValue(BatteryGConf::GConfKey, QVariant value);
    QVariant value(BatteryGConf::GConfKey);
    int keyCount();

signals:    
    void valueChanged(BatteryGConf::GConfKey key, QVariant value);

private slots:
    void keyValueChanged();
    void deleteSetterThread();

private: //methods
    QString mapGConfKey(BatteryGConf::GConfKey key);

private: //attributes
    QHash <BatteryGConf::GConfKey, DuiGConfItem *> duiGConfItems;

};


#endif // BATTERYGCONF_H
