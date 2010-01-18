/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef PROFILEDATAINTERFACE_H
#define PROFILEDATAINTERFACE_H

#include <QObject>
class Profile;

class ProfileDataInterface : public QObject
{
    Q_OBJECT

public:
    
    enum ProfileId {
        ProfileIdRinging = 0,
        ProfileIdSilent,
        ProfileIdBeep,
        ProfileIdLoud,
        // none must be last to teel how many profiles there are available
        ProfileIdNone 
    };

    struct ProfileData {
        ProfileId  profileId;
        QString    profileName;
        int        volumeLevel;
        bool       vibrationEnabled;
        bool       isActive;
    };

    ProfileDataInterface ();
    virtual ~ProfileDataInterface ();

signals:
    void currentProfile (int id);
    void vibrationValue (int id, bool value);
    void volumeLevel (int id, int value);

public:
    QString getCurrentProfileName ();
    int getCurrentProfile ();
    QList<ProfileData> getProfilesData ();
    void setVibration (int id, bool value);
    void setVolumeLevel (int id, int value);

public slots:
    void setProfile (int value);

private slots:
    void currentProfileNameChanged (const QString &prof);

private:
    int checkSilent (int id, int level);
    static ProfileId mapId (const QString &id);
    static QString mapId (int id);
    static QString id2Name (const QString &id);

private:
    Profile *m_ProfileAPI;
};

#endif
