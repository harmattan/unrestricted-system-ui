#ifndef PROFILEDATAINTERFACE_H
#define PROFILEDATAINTERFACE_H

#include <QObject>
class Profile;

class ProfileDataInterface : public QObject
{
    Q_OBJECT

public:

    struct ProfileData
    {
        int profileId;
        QString profileName;
        int volumeLevel;
        bool vibrationEnabled;
        bool isActive;
    };

    ProfileDataInterface();
    virtual ~ProfileDataInterface();

signals:
    void currentProfile(int id);
    void vibrationValue(int id, bool value);
    void volumeLevel(int id, int value);

public:
    QString getCurrentProfileName();
    int getCurrentProfile();
    QList<ProfileData> getProfilesData();
    void setProfile(int value);
    void setVibration(int id, bool value);
    void setVolumeLevel(int id, int value);

private:
    int checkSilent(int id, int level);
    static int mapId(QString id);
    static QString mapId(int id);
    static QString id2Name(QString id);

private:
    Profile* api;
};

#endif // PROFILEDATAINTERFACE_H
