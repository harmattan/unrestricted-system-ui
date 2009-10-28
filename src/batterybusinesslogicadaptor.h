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
    void PSMValueChanged(QString);
    void PSMAutoValueChanged(bool);
    void remainingTimeValuesChanged(QStringList);

public slots:
    Q_NOREPLY void setPSMValue(const QString &value);
    Q_NOREPLY void setPSMAutoValue(bool disabled);
    Q_NOREPLY void setPSMThresholdValue(const QString &value);
    QString PSMValue();
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
