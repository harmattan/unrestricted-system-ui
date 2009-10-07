#include "profilebrief.h"
#include "profilewidget.h"
#include "dcpwidgettypes.h"

ProfileBrief::ProfileBrief(ProfileWidget* widget) : widget(widget)
{
}

QString ProfileBrief::valueText() const
{
    if(widget)
    {
        return widget->currentProfile();
    }
    return "";
}



