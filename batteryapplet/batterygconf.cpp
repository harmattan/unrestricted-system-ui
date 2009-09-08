#include "batterygconf.h"

#include <DuiGConfItem>
#include <QDebug>

namespace {    
    QString Dir = "/battery/settings"; //TODO: Change the dir value if necessary
}

BatteryGConf::BatteryGConf()
{    
    // init the gconf keys    
    duiGConfItems.insert(BatteryGConf::PSMToggleKey, new DuiGConfItem(mapGConfKey(BatteryGConf::PSMToggleKey)));
    duiGConfItems.insert(BatteryGConf::PSMDisabledKey, new DuiGConfItem(mapGConfKey(BatteryGConf::PSMDisabledKey)));
    duiGConfItems.insert(BatteryGConf::PSMThresholdKey, new DuiGConfItem(mapGConfKey(BatteryGConf::PSMThresholdKey)));
    duiGConfItems.insert(BatteryGConf::PSMThresholdValuesKey, new DuiGConfItem(mapGConfKey(BatteryGConf::PSMThresholdValuesKey)));
    duiGConfItems.insert(BatteryGConf::RemainingTimesKey, new DuiGConfItem(mapGConfKey(BatteryGConf::RemainingTimesKey)));
    duiGConfItems.insert(BatteryGConf::BatteryLevelKey, new DuiGConfItem(mapGConfKey(BatteryGConf::BatteryLevelKey)));
    duiGConfItems.insert(BatteryGConf::ChargingKey, new DuiGConfItem(mapGConfKey(BatteryGConf::ChargingKey)));

    QHash<BatteryGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i)
        connect( i.value(), SIGNAL(valueChanged()), this, SLOT(keyValueChanged()));
}

BatteryGConf::~BatteryGConf()
{    
    QHash<BatteryGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i) {
        delete i.value();
        i.value() = NULL;
    }  
}

int BatteryGConf::keyCount()
{
    DuiGConfItem duiGConfItem(Dir);
    QList<QString> list = duiGConfItem.listEntries();
    return list.size();
}

void BatteryGConf::keyValueChanged()
{
    DuiGConfItem *duiGConfItem = static_cast<DuiGConfItem*>(this->sender());
    emit valueChanged(duiGConfItems.key(duiGConfItem), duiGConfItem->value());
}

void BatteryGConf::setValue(BatteryGConf::GConfKey key, QVariant value)
{
    duiGConfItems[key]->set(value);
}

QVariant BatteryGConf::value(BatteryGConf::GConfKey key)
{
    return duiGConfItems[key]->value();
}

QString BatteryGConf::mapGConfKey(BatteryGConf::GConfKey key)
{
    QString keyStr("%1%2");
    switch(key) {
        case BatteryGConf::PSMToggleKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryPSMToggle");
            break;
        case BatteryGConf::PSMDisabledKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryPSMDisabled");
            break;
        case BatteryGConf::PSMThresholdKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryPSMThreshold");
            break;
        case BatteryGConf::PSMThresholdValuesKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryPSMThresholdValues");
            break;
        case BatteryGConf::RemainingTimesKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryRemainingTimes");
            break;        
        case BatteryGConf::BatteryLevelKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryLevel");
            break;
        case BatteryGConf::ChargingKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryCharging");
            break;        
        default:
            break;
    }
    return keyStr;
}
