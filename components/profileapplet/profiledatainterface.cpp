/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "profiledatainterface.h"

#include <Profile>
#include <QDebug>
#include <QStringList>
#include <QList>

//#define DEBUG
#include "../debug.h"

//#define USE_TEST_DATA

using namespace ProfileName;


ProfileDataInterface::ProfileDataInterface ()
{
    m_ProfileAPI = new Profile ();

    SYS_DEBUG ("*** active profile = '%s'", 
            SYS_STR (m_ProfileAPI->activeProfile()));
    connect (m_ProfileAPI, SIGNAL(activeProfileChanged(QString)), 
            this, SLOT(currentProfileNameChanged(QString)));
}

ProfileDataInterface::~ProfileDataInterface ()
{
    delete m_ProfileAPI;
    m_ProfileAPI = NULL;
}


QString 
ProfileDataInterface::getCurrentProfileName ()
{
    qDebug() << Q_FUNC_INFO;
    QString prof = m_ProfileAPI->activeProfile();

    #ifdef USE_TEST_DATA
    return "Ringing";
    #endif

    return id2Name (prof);
}

void 
ProfileDataInterface::currentProfileNameChanged (
        const QString &prof)
{
    emit currentProfile(mapId(prof));
}

int 
ProfileDataInterface::getCurrentProfile ()
{
    qDebug() << Q_FUNC_INFO;

    QString prof = m_ProfileAPI->activeProfile();
    #ifdef USE_TEST_DATA
    return ProfileIdRinging;
    #endif
    return mapId (prof);
}

QList<ProfileDataInterface::ProfileData> 
ProfileDataInterface::getProfilesData ()
{
    qDebug() << Q_FUNC_INFO;
    QList<ProfileData> data;

    // send profile <name, is> map
    QStringList ids = m_ProfileAPI->profileNames ();

    // send...
    for (int i = 0; i < ids.count(); ++i) {
        ProfileDataInterface::ProfileData d;
        //get name...
        QString id = ids.at(i);
        d.profileId = mapId (id);
        d.profileName = id2Name (id);
        d.volumeLevel = checkSilent (
                d.profileId, m_ProfileAPI->volumeLevel(id));
        d.vibrationEnabled = m_ProfileAPI->isVibrationEnabled (id);
        data.append(d);
    }
    qDebug() << Q_FUNC_INFO << "data.count():" << data.count();

    #ifdef USE_TEST_DATA
    SYS_WARNING ("Adding test data.");
    {
        ProfileDataInterface::ProfileData d;
        //get name...
        QString id ("ringing");
        d.profileId = ProfileIdRinging;
        d.profileName = "Ringing";
        d.volumeLevel = 10;
        d.vibrationEnabled = false;
        data.append (d);
    } {
        ProfileDataInterface::ProfileData d;
        //get name...
        QString id ("silent");
        d.profileId = ProfileIdSilent;
        d.profileName = "Silent";
        d.volumeLevel = 0;
        d.vibrationEnabled = true;
        data.append (d);
    } {
        ProfileDataInterface::ProfileData d;
        //get name...
        QString id ("beep");
        d.profileId = ProfileIdBeep;
        d.profileName = "Beep";
        d.volumeLevel = 10;
        d.vibrationEnabled = false;
        data.append (d);
    } {
        ProfileDataInterface::ProfileData d;
        //get name...
        QString id ("loud");
        d.profileId = ProfileIdLoud;
        d.profileName = "Loud";
        d.volumeLevel = 100;
        d.vibrationEnabled = false;
        data.append (d);
    }
    #endif

    return data;
}

void 
ProfileDataInterface::setProfile (
        int value)
{
    qDebug() << Q_FUNC_INFO << "(" << value << ")";

    bool success = m_ProfileAPI->setActiveProfile (mapId(value));
    if (!success) {
        SYS_WARNING ("Failed setting profile.");
        // TODO: what??
    }
}

void 
ProfileDataInterface::setVibration (
        int    id, 
        bool   value)
{
    qDebug() << Q_FUNC_INFO << "(" << id << "," << value <<  ")";

    bool success = m_ProfileAPI->setVibration(mapId(id), value);
    if (!success) {
        SYS_WARNING ("Failed setting the vibration.");
        // TODO: what??
    }
}

void 
ProfileDataInterface::setVolumeLevel (
        int   id, 
        int   value)
{
    qDebug() << Q_FUNC_INFO << "(" << id << "," << value <<  ")";

    bool success = m_ProfileAPI->setVolumeLevel(mapId(id), value);
    if (!success) {
        SYS_WARNING ("Failed setting volume level");
        // TODO: what??
    }
}

int 
ProfileDataInterface::checkSilent (
        int   id, 
        int   level)
{
    if (id == ProfileIdSilent) {
        level = -1;
    }

    return level;
}

QString 
ProfileDataInterface::id2Name (
        const QString &id)
{
    QString localised = "";
    if (ProfileName::ringing == id) {
        //% "Ringing"
        localised = qtTrId ("qtn_prof_ringing");
    } else if (ProfileName::silent == id) {
        //% "Silent"
        localised = qtTrId ("qtn_prof_silent");
    } else if (ProfileName::beep == id) {
        //% "Beep"
        localised = qtTrId ("qtn_prof_beep");
    } else if (ProfileName::loud == id) {
        //% "Loud"
        localised = qtTrId ("qtn_prof_loud");
    }

    SYS_DEBUG ("returning '%s'", SYS_STR (localised));
    return localised;
}

ProfileDataInterface::ProfileId 
ProfileDataInterface::mapId (
        const QString &id)
{
    ProfileId intId = ProfileIdNone;

    if (ProfileName::ringing == id) {
        intId = ProfileIdRinging;
    } else if (ProfileName::silent == id) {
        intId = ProfileIdSilent;
    } else if (ProfileName::beep == id) {
        intId = ProfileIdBeep;
    } else if (ProfileName::loud == id) {
        intId = ProfileIdLoud;
    }

    return intId;
}

QString 
ProfileDataInterface::mapId (
        int id)
{
    QString stringId = "";

    switch (id) {
        case ProfileIdRinging:
            stringId = ProfileName::ringing;
            break;
        case ProfileIdSilent:
            stringId = ProfileName::silent;
            break;
        case ProfileIdBeep:
            stringId = ProfileName::beep;
            break;
        case ProfileIdLoud:
            stringId = ProfileName::loud;
            break;
    }

    return stringId;
}


