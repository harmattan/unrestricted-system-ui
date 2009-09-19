#ifndef SYSTEMUIGCONF_H
#define SYSTEMUIGCONF_H

#include <QHash>
#include <QObject>
#include <QVariant>

class DuiGConfItem;

class SystemUIGConf : public QObject
{
    Q_OBJECT
public:

    enum GConfKeyGroup {
        Battery,
        Display
    };

    enum GConfKey {
        BatteryPSMToggleKey,
        BatteryPSMDisabledKey,
        BatteryPSMThresholdKey,
        DisplayBrightnessKey,
        DisplayScreenLightsKey,
        DisplayScreenLightsToggleKey
    };   

    SystemUIGConf();
    virtual ~SystemUIGConf();

    void setValue(SystemUIGConf::GConfKey key, QVariant value);
    QVariant value(SystemUIGConf::GConfKey);
    int keyCount(SystemUIGConf::GConfKeyGroup keyGroup);

signals:
    void valueChanged(SystemUIGConf::GConfKey key, QVariant value);

private slots:
    void keyValueChanged();

private: //methods
    QString mapGConfKey(SystemUIGConf::GConfKey key);

private: //attributes
    QHash<SystemUIGConf::GConfKey, DuiGConfItem *> duiGConfItems;

};


#endif // SYSTEMGCONF_H
