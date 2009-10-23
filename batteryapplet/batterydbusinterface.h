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
    void setPSMValue(const QString &value);

public slots:
    void remainingTimeValuesRequired();
    void batteryBarValueRequired();
    void PSMThresholdValueRequired();
    void setPSMAutoValue(bool toggle);
    void setPSMThresholdValue(const QString &value);

public slots:    
    void DBusMessagingFailure();
    void valueSet();
    void querySent();

signals:
    void batteryCharging(int);
    void batteryNotCharging();
    void batteryBarValueChanged(int);
    void PSMValueReceived(QString);
    void PSMAutoValueReceived(bool);
    void PSMThresholdValuesReceived(QStringList);
    void PSMThresholdValueReceived(QString);
    void remainingTimeValuesReceived(QStringList);
    void batteryBarValueReceived(int);

private:
    QDBusInterface *dbusIf;
};

#endif // BATTERYDBUSINTERFACE_H
