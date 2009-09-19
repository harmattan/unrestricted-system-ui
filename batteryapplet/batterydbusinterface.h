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

    void PSMToggleValueRequired();
    void PSMDisabledValueRequired();
    void batteryChargingStateRequired();
    void PSMThresholdValuesRequired();
    void PSMThresholdValueRequired();
    void batteryLevelValueRequired();
    void setPSMThresholdValue(const QString &value);
    void setPSMToggleValue(bool toggle);
    void setPSMDisabledValue(bool disabled);

public slots:
    void remainingTimeValuesRequired();

private slots:
    void batteryChargingStateReceived(bool state);
    void DBusMessagingFailure();
    void valueSet();

signals:
    void batteryCharging();
    void batteryNotCharging();
    void batteryLevelValueChanged(int);
    void PSMToggleValueReceived(bool);
    void PSMDisabledValueReceived(bool);
    void PSMThresholdValuesReceived(QStringList);
    void PSMThresholdValueReceived(QString);
    void remainingTimeValuesReceived(QStringList);
    void batteryLevelValueReceived(int);

private:
    QDBusInterface *dbusIf;
};

#endif // BATTERYDBUSINTERFACE_H
