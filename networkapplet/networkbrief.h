#ifndef NETWORKBRIEF_H
#define NETWORKBRIEF_H

#include "dcpbrief.h"

class NetworkBrief: public DcpBrief
{
    Q_OBJECT

public:
    virtual QString valueText() const;

};

#endif // NETWORKBRIEF
