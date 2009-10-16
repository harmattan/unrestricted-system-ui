#include "profilebrief.h"
#include "dcpwidgettypes.h"
#include "profiledbusinterface.h"

ProfileBrief::ProfileBrief()
{
}

QString ProfileBrief::valueText() const
{
    ProfileDBusInterface* profileIf = new ProfileDBusInterface();
    QString name = profileIf->getCurrentProfileName();
    delete profileIf;
    return name;
}


