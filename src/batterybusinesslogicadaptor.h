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
    void batteryLevelValueChanged(int);
    void PSMValueChanged(QString);
    void PSMAutoValueChanged(bool);

public slots:
    Q_NOREPLY void setPSMValue(const QString &value);
    Q_NOREPLY void setPSMAutoValue(bool disabled);
    Q_NOREPLY void setPSMThresholdValue(const QString &value);
    QString PSMValue();
    bool PSMAutoValue();
    bool batteryChargingState();   
    int batteryLevelValue();
    QString PSMThresholdValue();
    QStringList PSMThresholdValues();
    QStringList remainingTimeValues();

private:
    BatteryBusinessLogic *batteryLogic;
};

#endif // BATTERYBUSINESSLOGICADAPTOR_H
