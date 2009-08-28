#include "batterygconf.h"

//temp
#include "batterygconflistener.h"

#include <DuiConf>
#include <QVariant>
#include <QDebug>

namespace {    
    QString Dir = "/tmp"; //TODO: Change the dir  value
}

BatteryGConf::BatteryGConf(QObject *listener, QHash<BatteryGConf::GConfKey, QString> &keysAndSlots)
{
    duiConf = new DuiConf(Dir,true);

    //temp
    BatteryGConfListener *listener2 = new BatteryGConfListener();
    duiConf->notifyAdd("/tmp/zuke", listener2, "test");
    float jaakko = 2.0;
    duiConf->set("/tmp/zuke", jaakko);

    QHash<BatteryGConf::GConfKey,QString>::const_iterator i;
    for (i = keysAndSlots.begin(); i != keysAndSlots.end(); ++i)
        duiConf->notifyAdd(mapGConfKey(i.key()), listener, i.value().toLatin1());
}

BatteryGConf::~BatteryGConf()
{    
    delete duiConf;
    duiConf = NULL;    
}

void BatteryGConf::setPSMToggle(bool toggle)
{    
    duiConf->set(mapGConfKey(BatteryGConf::PSMToggleKey), toggle);
}

void BatteryGConf::setPSMDisabled(bool disabled)
{
    duiConf->set(mapGConfKey(BatteryGConf::PSMDisabledKey), disabled);
}

void BatteryGConf::setPSMThreshold(int threshold)
{
    duiConf->set(mapGConfKey(BatteryGConf::PSMThresholdKey), threshold);
}

void BatteryGConf::setPSMThresholdValues(QList<int> values)
{    
    duiConf->set(mapGConfKey(BatteryGConf::PSMThresholdValueCountKey), values.size());
    QString key(mapGConfKey(BatteryGConf::PSMThresholdValueNKey));
    for (int i = 0; i < values.size(); ++i)
        duiConf->set(key.arg(i), values.at(i));        

}

void BatteryGConf::setRemainingTalkTime(int time)
{
    duiConf->set(mapGConfKey(BatteryGConf::RemainingTalkTimeKey), time);
}

void BatteryGConf::setRemainingStandByTime(int time)
{
    duiConf->set(mapGConfKey(BatteryGConf::RemainingStandByTimeKey), time);
}

void BatteryGConf::setBatteryLevel(int level)
{
    duiConf->set(mapGConfKey(BatteryGConf::BatteryLevelKey), level);
}

void BatteryGConf::setCharging(bool charging)
{
    duiConf->set(mapGConfKey(BatteryGConf::ChargingKey), charging);
}

bool BatteryGConf::PSMToggle()
{
    QVariant toggle;
    duiConf->getValue(mapGConfKey(BatteryGConf::PSMToggleKey), toggle);
    return toggle.toBool();
}

bool BatteryGConf::PSMDisabled()
{
    QVariant disabled;
    duiConf->getValue(mapGConfKey(BatteryGConf::PSMDisabledKey), disabled);
    return disabled.toBool();
}

int BatteryGConf::PSMThreshold()
{
    QVariant threshold;
    duiConf->getValue(mapGConfKey(BatteryGConf::PSMThresholdKey), threshold);
    return threshold.toInt();
}

QList<int> BatteryGConf::PSMThresholdValues()
{
    QVariant count;
    QVariant value;
    QList<int> values;

    duiConf->getValue(mapGConfKey(BatteryGConf::PSMThresholdValueCountKey), count);
    QString key(mapGConfKey(BatteryGConf::PSMThresholdValueNKey));
    for (int i = 0; i < count.toInt(); ++i) {
        duiConf->getValue(key.arg(i), value);
        values << value.toInt();
    }
    return values;
}

int BatteryGConf::remainingTalkTime()
{
    QVariant time;
    duiConf->getValue(mapGConfKey(BatteryGConf::RemainingTalkTimeKey), time);
    return time.toInt();
}

int BatteryGConf::remainingStandByTime()
{
    QVariant time;
    duiConf->getValue(mapGConfKey(BatteryGConf::RemainingStandByTimeKey), time);
    return time.toInt();
}

int BatteryGConf::batteryLevel()
{
    QVariant level;
    duiConf->getValue(mapGConfKey(BatteryGConf::BatteryLevelKey), level);
    return level.toInt();
}

bool BatteryGConf::charging()
{
    QVariant charging;
    duiConf->getValue(mapGConfKey(BatteryGConf::ChargingKey), charging);
    return charging.toBool();
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
        case BatteryGConf::PSMThresholdValueCountKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryPSMThresholdValueCount");
        break;
        case BatteryGConf::PSMThresholdValueNKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryPSMThresholdValue%1");
        break;
        case BatteryGConf::RemainingTalkTimeKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryTalkTime");
            break;
        case BatteryGConf::RemainingStandByTimeKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryStandByTime");
            break;
        case BatteryGConf::BatteryLevelKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryLevel");
            break;
        case BatteryGConf::ChargingKey:
            keyStr = keyStr.arg(Dir).arg("/BatteryCharging");
            break;
    }
    return keyStr;
}
