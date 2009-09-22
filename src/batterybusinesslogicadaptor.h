#ifndef BATTERYBUSINESSLOGICADAPTOR_H
#define BATTERYBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

class BatteryBusinessLogic;

class BatteryBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.battery")

public:
    BatteryBusinessLogicAdaptor(QObject *obj, BatteryBusinessLogic *batteryLogic);

signals:
    void batteryCharging();
    void batteryNotCharging();
    void batteryLevelValueChanged(int level);
    void PSMToggleValueChanged(bool toggle);

public slots:
    Q_NOREPLY void setPSMToggleValue(bool toggle);
    Q_NOREPLY void setPSMDisabledValue(bool disabled);
    Q_NOREPLY void setPSMThresholdValue(const QString &values);
    bool PSMToggleValue();
    bool PSMDisabledValue();
    bool batteryChargingState();   
    int batteryLevelValue();
    QString PSMThresholdValue();
    QStringList PSMThresholdValues();
    QStringList remainingTimeValues();

private:
    BatteryBusinessLogic *batteryLogic;
};

#endif // BATTERYBUSINESSLOGICADAPTOR_H
