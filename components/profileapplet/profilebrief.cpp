#include "profilebrief.h"
#include "dcpwidgettypes.h"
#include "profiledatainterface.h"

ProfileBrief::ProfileBrief()
{
}

QString ProfileBrief::valueText() const
{
    ProfileDataInterface profileIf;
    return profileIf.getCurrentProfileName();
}


