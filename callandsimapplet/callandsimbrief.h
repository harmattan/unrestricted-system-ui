#ifndef CALLANDSIMBRIEF_H
#define CALLANDSIMBRIEF_H

#include "dcpbrief.h"

class CallAndSimBrief : public DcpBrief
{
    Q_OBJECT

public:
    CallAndSimBrief();

    virtual QString valueText() const;
};

#endif // CALLANDSIMBRIEF_H
