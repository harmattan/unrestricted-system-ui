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
    ~BatteryBusinessLogicAdaptor();

signals:
    void batteryCharging(int);
    void batteryNotCharging();
    void batteryBarValueChanged(int);
    void PSMValueChanged(bool);
    void PSMAutoDisabled();
    void remainingTimeValuesChanged(QStringList);

public slots:
    Q_NOREPLY void setPSMValue(bool toggle);
    Q_NOREPLY void setPSMAutoValue(bool disabled);
    Q_NOREPLY void setPSMThresholdValue(const QString &value);
    bool PSMValue();
    bool PSMAutoValue();
    void batteryChargingState();
    int batteryBarValue();
    QString PSMThresholdValue();
    QStringList PSMThresholdValues();
    QStringList remainingTimeValues();

private:
    BatteryBusinessLogic *batteryLogic;
};

#endif // BATTERYBUSINESSLOGICADAPTOR_H
