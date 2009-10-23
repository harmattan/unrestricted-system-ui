#include "profilebrief.h"
#include "dcpwidgettypes.h"
#include "profiledatainterface.h"

ProfileBrief::ProfileBrief()
{
}

QString ProfileBrief::valueText() const
{
    ProfileDataInterface* profileIf = new ProfileDataInterface();
    QString name = profileIf->getCurrentProfileName();
    delete profileIf;
    return name;
}


