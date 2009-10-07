#ifndef PROFILEBRIEF_H
#define PROFILEBRIEF_H

#include "dcpbrief.h"
#include <QPointer>

class ProfileWidget;

class ProfileBrief: public DcpBrief
{
    Q_OBJECT

public:
    ProfileBrief(ProfileWidget* widget);
    virtual QString valueText() const;
private:
    QPointer<ProfileWidget> widget;
};

#endif // PROFILEBRIEF
