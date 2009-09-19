#ifndef DISPLAYBRIEF_H
#define DISPLAYBRIEF_H

#include "dcpbrief.h"

class DisplayBrief: public DcpBrief
{
    Q_OBJECT

public:
    virtual QString valueText() const;

};

#endif // DISPLAYBRIEF
