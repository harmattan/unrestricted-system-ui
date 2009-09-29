#ifndef BATTERYDBUSINTERFACE_H
#define BATTERYDBUSINTERFACE_H

#include <QObject>
#include <QStringList>

class QDBusInterface;

class BatteryDBusInterface : public QObject
{
    Q_OBJECT

public:
    BatteryDBusInterface();
    virtual ~BatteryDBusInterface();

    void PSMValueRequired();
    void PSMAutoValueRequired();
    void batteryChargingStateRequired();
    void PSMThresholdValuesRequired();
    void PSMThresholdValueRequired();
    void batteryLevelValueRequired();
    void setPSMThresholdValue(const QString &value);
    void setPSMValue(bool toggle);
    void setPSMAutoValue(bool toggle);        

public slots:
    void remainingTimeValuesRequired();

public slots:
    void batteryChargingStateReceived(bool state);
    void DBusMessagingFailure();
    void valueSet();

signals:
    void batteryCharging();
    void batteryNotCharging();
    void batteryLevelValueChanged(int);
    void PSMValueReceived(bool);
    void PSMAutoValueReceived(bool);
    void PSMThresholdValuesReceived(QStringList);
    void PSMThresholdValueReceived(QString);
    void remainingTimeValuesReceived(QStringList);
    void batteryLevelValueReceived(int);

private:
    QDBusInterface *dbusIf;
};

#endif // BATTERYDBUSINTERFACE_H
