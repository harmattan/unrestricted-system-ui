/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LEDBRIEF_H
#define LEDBRIEF_H

#include "dcpbrief.h"

class LedDBusInterface;

class LedBrief: public DcpBrief
{
    Q_OBJECT

public:
    LedBrief (LedDBusInterface *dbusIf);

public slots:
    void ledStateReceived (bool brief);

public:
    virtual int widgetTypeID () const;
    virtual bool toggle () const;
    virtual void setToggle (bool toggle);

private:
    LedDBusInterface *m_LedDBusIf;
    bool              m_LedState;
};

#endif
