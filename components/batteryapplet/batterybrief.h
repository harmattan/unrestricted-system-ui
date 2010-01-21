/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef BATTERYBRIEF_H
#define BATTERYBRIEF_H

#include "dcpbrief.h"

class BatteryDBusInterface;

class BatteryBrief: public DcpBrief
{
    Q_OBJECT

public:
    BatteryBrief ();
    ~BatteryBrief ();

    virtual QString valueText() const;

private slots:
    void batteryCharging (int);
    void batteryNotCharging ();

private:
    BatteryDBusInterface  *m_BatteryIf;
    bool                   m_Charging;
    bool                   m_NotCharging;
};

#endif
