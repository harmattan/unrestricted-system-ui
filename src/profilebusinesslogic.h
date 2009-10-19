#ifndef PROFILEBUSINESSLOGIC_H
#define PROFILEBUSINESSLOGIC_H

#include <QObject>
#include <QStringList>

class Profile;

class ProfileBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ProfileBusinessLogic();
    virtual ~ProfileBusinessLogic();

signals:
    void currentProfile(int id);
    void vibrationValue(int id, bool value);
    void volumeLevel(int id, int value);

public:
    QString getCurrentProfileName();
    void getCurrentProfile();
    void getVolumeLevels();
    void getVibrationValues();
    void setProfile(int value);
    void setVibration(int id, bool value);
    void setVolumeLevel(int id, int value);

private:
    static int name2Id(QString profileName);
    static QString id2Name(int profileId);
    static QString id2LocName(QString id);

private:
    Profile* api;
};

#endif // PROFILEBUSINESSLOGIC_H
