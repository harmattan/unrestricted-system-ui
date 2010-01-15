#ifndef PROFILEBRIEF_H
#define PROFILEBRIEF_H

#include "dcpbrief.h"

class ProfileBrief: public DcpBrief
{
    Q_OBJECT

public:
    ProfileBrief();
    virtual QString valueText() const;

};

#endif // PROFILEBRIEF
