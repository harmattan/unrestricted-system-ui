/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ledbusinesslogicadaptor.h"
#include "ledbusinesslogic.h"

//#define DEBUG
#include "../debug.h"

LedBusinessLogicAdaptor::LedBusinessLogicAdaptor (
            QObject          *parent, 
            LedBusinessLogic *ledLogic) :
    QDBusAbstractAdaptor (parent),
    m_LedLogic (ledLogic)
{
    SYS_DEBUG ("");

    connect (ledLogic, SIGNAL(ledsStateChanged(bool)),
            this, SIGNAL(ledsStateChanged (bool)));
    connect (ledLogic, SIGNAL(illuminationLedStateChanged(bool)),
            this, SIGNAL(illuminationLedStateChanged(bool)));
    connect (ledLogic, SIGNAL(eventsLedStateChanged(bool)),
            this, SIGNAL(eventsLedStateChanged(bool)));
}

void
LedBusinessLogicAdaptor::setLedsEnabled (
        bool         enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_LedLogic->setLedsEnabled (enabled);
}

void
LedBusinessLogicAdaptor::setIlluminationLedEnabled (
        bool         enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_LedLogic->setIlluminationLedEnabled (enabled);
}

void
LedBusinessLogicAdaptor::setEventsLedEnabled (
        bool         enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_LedLogic->setEventsLedEnabled (enabled);
}

bool
LedBusinessLogicAdaptor::LedsEnabled ()
{
    SYS_DEBUG ("");
    return m_LedLogic->ledsEnabled ();
}

bool
LedBusinessLogicAdaptor::IlluminationLedEnabled ()
{
    SYS_DEBUG ("");
    return m_LedLogic->illuminationLedEnabled ();
}

bool
LedBusinessLogicAdaptor::EventsLedEnabled ()
{
    SYS_DEBUG ("");
    return m_LedLogic->eventsLedEnabled ();
}

