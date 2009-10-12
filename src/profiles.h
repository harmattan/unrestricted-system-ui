#ifndef PROFILES_H
#define PROFILES_H

#include <QObject>
#include <QStringList>

class QDBusInterface;

class ProfileData
{
public:
    ProfileData(QString n = "", int l=-1, bool v=true){profileName=n; volumeLevel=l; vibrationEnabeld=v;}

    QString profileName;
    int volumeLevel;
    bool vibrationEnabeld;
};

class Profiles : QObject
{
    Q_OBJECT

public:
    Profiles();
    ~Profiles();

public slots:

    /**
     * Get active profile name.
     *
     * @param n/a
     * @returns profile : STRING
     **/
    QString activeProfile();

    /**
     * Set active profile name.
     *
     * @param profile : STRING
     * @returns success : BOOLEAN
     **/
    bool setActiveProfile(QString name);

    /**
     * Get available profiles.
     *
     * @param n/a
     * @returns profiles : ARRAY of STRING
     **/
    QStringList getProfiles();

    /**
     * Get profile value.
     *
     * @param profile : STRING
     * @param key     : STRING
     *
     * @returns value : STRING
     **/
    QStringList getVolumeRange();
    int getVolumeLevel(QString profileName);
    bool isVibrationEnabled(QString profileName);

    /**
     * Set profile value.
     *
     * @param   profile : STRING
     * @param   key     : STRING
     * @param   val     : STRING
     *
     * @returns success : BOOLEAN
     **/
    bool setVolumeLevel(QString profileName, int level);
    bool setVibration(QString profileName, bool enabled);

    /**
     * Get all profile values.
     *
     * @param   profile : STRING
     *
     * @returns values : ARRAY of STRUCT
     *         <br> key  : STRING
     *         <br> val  : STRING
     *         <br> type : STRING
     **/
    ProfileData getProfile(QString profileName);

signals:

    /**
     * Signal emitted after changes to profile data
     *
     * @param changed : BOOLEAN
     * @param active  : BOOLEAN
     * @param profile : STRING
     * @param values  : ARRAY of STRUCT
     *         <br> key  : STRING
     *         <br> val  : STRING
     *         <br> type : STRING
     **/
    void profileChanged(ProfileData data);
    void activeProfileChanged(QString newActiveProfileName);

private:
    QDBusInterface* managerIf;
};

#endif // PROFILES_H
