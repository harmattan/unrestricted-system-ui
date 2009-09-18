#ifndef BATTERYDBUSINTERFACE_H
#define BATTERYDBUSINTERFACE_H

#include <QObject>
#include <QVariant>
#include <QStringList>

class QDBusInterface;

class BatteryDBusInterface : public QObject
{
    Q_OBJECT

public:
    enum DBusDataType {
        DBusDataTypeVoid = 0,
        DBusDataTypeBool,
        DBusDataTypeInt,
        DBusDataTypeQString,
        DBusDataTypeQStringList
    };

    BatteryDBusInterface();
    virtual ~BatteryDBusInterface();

    void PSMToggleValueRequired();
    void PSMDisabledValueRequired();
    void BatteryChargingStateRequired();
    void PSMThresholdValuesRequired();
    void PSMThresholdValueRequired();
    void batteryLevelValueRequired();
    void setPSMThresholdValue(const QString &value);
    void setPSMToggleValue(bool toggle);
    void setPSMDisabledValue(bool disabled);

    QVariant call(const QString &method, BatteryDBusInterface::DBusDataType dataType, const QVariant &param1 = QVariant(),
                  const QVariant &param2 = QVariant(), const QVariant &param3 = QVariant(), const QVariant &param4 = QVariant());

public slots:
    void remainingTimeValuesRequired();

private slots:
    void PSMToggleValueReceived(bool value);    
    void PSMDisabledValueReceived(bool value);    
    void BatteryChargingStateReceived(bool state);
    void PSMThresholdValuesReceived(const QStringList &values);    
    void PSMThresholdValueReceived(const QString &value);
    void remainingTimeValuesReceived(const QStringList &values);
    void batteryLevelValueReceived(int value);
    void DBusMessagingFailure();
    void valueSet();

signals:
    void batteryCharging();
    void batteryNotCharging();
    void batteryLevelValueChanged(int level);
    void PSMToggleValueChanged(bool value);
    void PSMDisabledValueChanged(bool value);
    void PSMThresholdValuesAvailable(QStringList values);
    void PSMThresholdValueAvailable(QString value);
    void remainingTimeValuesAvailable(QStringList values);
    void batteryLevelValueAvailable(int value);

private:
    QDBusInterface *dbusIf;
};

#endif // BATTERYDBUSINTERFACE_H
