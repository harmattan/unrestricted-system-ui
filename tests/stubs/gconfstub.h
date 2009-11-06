#ifndef SYSTEMUIGCONF_H
#define SYSTEMUIGCONF_H

#include <QObject>
#include <QVariant>

class SystemUIGConf : public QObject
{
public:
     enum GConfKeyGroup {
        Battery,
        Display,
        Network
    };

    enum GConfKey {
        BatteryPSMAutoKey,
        BatteryPSMThresholdKey,
        DisplayBrightnessMaxLevelKey,
        DisplayBrightnessLevelStepKey,
        DisplayBrightnessKey,
        DisplayBlankTimeoutsKey,
        DisplayBlankTimeoutKey,
        DisplayDimTimeoutsKey,
        DisplayDimTimeoutKey,
        DisplayBlankInhibitKey,
        NetworkRoamingKey,
        NetworkRoamingUpdatesKey
    };

    SystemUIGConf(QObject *parent = 0);
    ~SystemUIGConf();

    void setValue(SystemUIGConf::GConfKey key, QVariant value);
    QVariant value(SystemUIGConf::GConfKey, QVariant def = NULL);
    int keyCount(SystemUIGConf::GConfKeyGroup keyGroup);

private: //attributes
    QHash<SystemUIGConf::GConfKey, QVariant> GConfValues;

};

#endif // SYSTEMUIGCONF_H
