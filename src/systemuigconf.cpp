#include "systemuigconf.h"

#include <DuiGConfItem>
#include <QDebug>

SystemUIGConf::SystemUIGConf(QObject* parent) :
        QObject(parent)
{
    // init the gconf keys    
    duiGConfItems.insert(SystemUIGConf::BatteryPSMAutoKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMAutoKey)));
    duiGConfItems.insert(SystemUIGConf::BatteryPSMThresholdKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMThresholdKey)));
    duiGConfItems.insert(SystemUIGConf::OfflineModeKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::OfflineModeKey)));
    duiGConfItems.insert(SystemUIGConf::OfflinePhoneNetworkStateKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::OfflinePhoneNetworkStateKey)));

    QHash<SystemUIGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i)
        connect( i.value(), SIGNAL(valueChanged()), this, SLOT(keyValueChanged()));

}

SystemUIGConf::~SystemUIGConf()
{
    QHash<SystemUIGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i) {
        delete i.value();
        i.value() = NULL;
    }
}

int SystemUIGConf::keyCount(SystemUIGConf::GConfKeyGroup keyGroup)
{    
    DuiGConfItem duiGConfItem(mapGConfKeyGroup(keyGroup));
    QList<QString> list = duiGConfItem.listEntries();
    return list.size();
}

void SystemUIGConf::setValue(SystemUIGConf::GConfKey key, QVariant value)
{
    duiGConfItems.value(key)->set(value);
}

QVariant SystemUIGConf::value(SystemUIGConf::GConfKey key, QVariant def)
{
    if(def.isNull())
        return duiGConfItems.value(key)->value();
    else
        return duiGConfItems.value(key)->value(def);
}

void SystemUIGConf::keyValueChanged()
{
    DuiGConfItem *item = static_cast<DuiGConfItem *>(this->sender());
    emit valueChanged(duiGConfItems.key(item), item->value());
}

QString SystemUIGConf::mapGConfKeyGroup(SystemUIGConf::GConfKeyGroup keyGroup)
{
    QString keyGroupStr;
    switch(keyGroup) {
        case SystemUIGConf::Battery:
            keyGroupStr = "/systemui/settings/battery";
            break;
        case SystemUIGConf::Offline:
            keyGroupStr = "/systemui/settings/offline";
            break;        
        default:
            break;
    }
    return keyGroupStr;
}

QString SystemUIGConf::mapGConfKey(SystemUIGConf::GConfKey key)
{
    QString keyStr("%1%2");
    switch(key) {        
        case SystemUIGConf::BatteryPSMAutoKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Battery)).arg("/batteryPSMAuto");
            break;
        case SystemUIGConf::BatteryPSMThresholdKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Battery)).arg("/batteryPSMThreshold");
            break;
        case SystemUIGConf::OfflineModeKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Offline)).arg("/offlineMode");
            break;
        case SystemUIGConf::OfflinePhoneNetworkStateKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Offline)).arg("/offlinePhoneNetworkState");
            break;
        default:
            break;
    }
    return keyStr;
}
