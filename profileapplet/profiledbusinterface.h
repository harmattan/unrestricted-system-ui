#ifndef PROFILEDBUSINTERFACE_H
#define PROFILEDBUSINTERFACE_H

#include <QObject>
#include <QStringList>

class QDBusInterface;

class ProfileDBusInterface : public QObject
{
    Q_OBJECT

public:
    ProfileDBusInterface();
    virtual ~ProfileDBusInterface();

    void currentProfileRequired();
    void volumeLevelsRequired();
    void vibrationValuesRequired();
    void setProfile(int profileId);
    void setVibration(int profileId, bool enabled);
    void setVolumeLevel(int profileId, int valueIndex);

private slots:
    void valueSet();
    void querySent();
    void DBusMessagingFailure();

signals:
    void currentProfile(int profileId);
    void volumeLevels(QStringList valueList);
    void volumeLevel(int profileId, int valueIndex);
    void vibrationValue(int profileId, bool enabled);

private:
    QDBusInterface *dbusIf;

};

#endif // PROFILEDBUSINTERFACE_H
