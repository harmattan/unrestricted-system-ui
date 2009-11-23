#include "profiledatainterface.h"

#include <Profile>
#include <DuiLocale>
#include <QStringList>

ProfileDataInterface::ProfileDataInterface() :
        QObject(),
        profileAPI(NULL)
{
    profileAPI = new Profile(this);
}

ProfileDataInterface::~ProfileDataInterface()
{
    delete profileAPI;
    profileAPI = NULL;
}

QStringList ProfileDataInterface::profileNames()
{
    return profileAPI->profileNames();
}

QString ProfileDataInterface::currentProfileName()
{
    return profileAPI->activeProfile();
}

bool ProfileDataInterface::setProfile(const QString &profileName)
{
    return profileAPI->setActiveProfile(profileName);
}

QString ProfileDataInterface::profileName2Text(const QString &profileName)
{
    QString text = "";
    if(profileName == ProfileName::ringing)
        text = trid("qtn_prof_ringing", "Ringing");
    else if(profileName == ProfileName::silent)
        text = trid("qtn_prof_silent", "Silent");
    else if(profileName == ProfileName::beep)
        text = trid("qtn_prof_beep", "Beep");
    else if(profileName == ProfileName::loud)
        text = trid("qtn_prof_loud", "Loud");
    return text;
}
