#include "profiledatainterface.h"
#include "profiletranslation.h"

#include <Profile>
#include <QDebug>
#include <QStringList>
#include <QList>

using namespace ProfileName;

namespace ProfileId{
    // must be identical with enum defined in ../profileapplet/profilebuttons.h
    enum ProfileId {
        ringing = 0,
        silent,
        beep,
        loud,
        none // none must be last to teel how many profiles there are available
    };    
}

ProfileDataInterface::ProfileDataInterface()
{
    api = new Profile(this);
    connect(api, SIGNAL(activeProfileChanged(QString)), this, SLOT(currentProfileNameChanged(QString)));    
}

ProfileDataInterface::~ProfileDataInterface()
{
}

QString ProfileDataInterface::getCurrentProfileName()
{
    qDebug() << Q_FUNC_INFO;
    QString prof = api->activeProfile();
    return id2Name(prof);
}

void ProfileDataInterface::currentProfileNameChanged(const QString &prof)
{
    emit currentProfile(mapId(prof));
}

int ProfileDataInterface::getCurrentProfile()
{
    qDebug() << Q_FUNC_INFO;
    QString prof = api->activeProfile();
    return mapId(prof);
}

QList<ProfileDataInterface::ProfileData> ProfileDataInterface::getProfilesData()
{
    qDebug() << Q_FUNC_INFO;
    QList<ProfileData> data;

    // send profile <name, is> map
    QStringList ids = api->profileNames();

    // send...
    for(int i = 0; i < ids.count(); ++i)
    {
        ProfileDataInterface::ProfileData d;
        //get name...
        QString id = ids.at(i);
        d.profileId = mapId(id);
        d.profileName = id2Name(id);
        d.volumeLevel = checkSilent(d.profileId, api->volumeLevel(id));
        d.vibrationEnabled = api->isVibrationEnabled(id);
        data.append(d);
    }
    qDebug() << Q_FUNC_INFO << "data.count():" << data.count();
    return data;
}

void ProfileDataInterface::setProfile(int value)
{
    qDebug() << Q_FUNC_INFO << "(" << value << ")";
    bool success = api->setActiveProfile(mapId(value));
    if(!success){
        // TODO: what??
    }
}

void ProfileDataInterface::setVibration(int id, bool value)
{
    qDebug() << Q_FUNC_INFO << "(" << id << "," << value <<  ")";
    bool success = api->setVibration(mapId(id), value);
    if(!success){
        // TODO: what??
    }
}

void ProfileDataInterface::setVolumeLevel(int id, int value)
{
    qDebug() << Q_FUNC_INFO << "(" << id << "," << value <<  ")";
    bool success = api->setVolumeLevel(mapId(id), value);
    if(!success){
        // TODO: what??
    }
}

int ProfileDataInterface::checkSilent(int id, int level)
{
    if(id == ProfileId::silent){
        level = -1;
    }
    return level;
}

QString ProfileDataInterface::id2Name(QString id)
{
    QString localised = "";
    if(ProfileName::ringing == id){
        localised = DcpProfile::RingingText;
    }else if(ProfileName::silent == id){
        localised = DcpProfile::SilentText;
    }else if(ProfileName::beep == id){
        localised = DcpProfile::BeepText;
    }else if(ProfileName::loud == id){
        localised = DcpProfile::LoudText;
    }
    return localised;
}

int ProfileDataInterface::mapId(QString id)
{
    int intId = ProfileId::none;
    if(ProfileName::ringing == id){
        intId = ProfileId::ringing;
    }else if(ProfileName::silent == id){
        intId = ProfileId::silent;
    }else if(ProfileName::beep == id){
        intId = ProfileId::beep;
    }else if(ProfileName::loud == id){
        intId = ProfileId::loud;
    }
    return intId;
}

QString ProfileDataInterface::mapId(int id)
{
    QString stringId = "";
    switch(id){
        case ProfileId::ringing:
            stringId = ProfileName::ringing;
            break;
        case ProfileId::silent:
            stringId = ProfileName::silent;
            break;
        case ProfileId::beep:
            stringId = ProfileName::beep;
            break;
        case ProfileId::loud:
            stringId = ProfileName::loud;
            break;
    }
    return stringId;
}


