/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ledbrief.h"
#include "dcpwidgettypes.h"
#include "leddbusinterface.h"

//#define DEBUG
#include "../debug.h"

LedBrief::LedBrief (
        LedDBusInterface *dbusIf) :
    m_LedDBusIf (dbusIf),
    m_LedState (false)
{
    Q_ASSERT (dbusIf);

    connect (dbusIf, SIGNAL(ledStateReceived(bool)),
            this, SLOT (ledStateReceived(bool)));

    dbusIf->ledStateRequired ();
}

int 
LedBrief::widgetTypeID () const
{
    return DcpWidgetType::Toggle;
}

bool
LedBrief::toggle () const
{
    return m_LedState;
}

void
LedBrief::setToggle (
        bool toggle)
{
    if (toggle == m_LedState)
        return;

    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");
    m_LedDBusIf->setLedState (toggle);
}

void
LedBrief::ledStateReceived (
        bool state)
{
    if (m_LedState == state)
        return;

    SYS_DEBUG ("state = %s", state ? "on" : "off");
    m_LedState = state;
    emit valuesChanged ();
}
