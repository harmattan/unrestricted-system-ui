/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ledbusinesslogic.h"

#define DEBUG
#include "../debug.h"

LedBusinessLogic::LedBusinessLogic (
        SystemUIGConf *systemUIGConf, 
        QObject       *parent) :
    QObject (parent),
    m_SystemUIGConf (systemUIGConf),
    m_Led (new QmLED)
{
    SYS_DEBUG ("");
}

LedBusinessLogic::~LedBusinessLogic ()
{
    delete m_Led;
    m_Led = 0;
}

bool
LedBusinessLogic::ledsEnabled ()
{
    return m_SystemUIGConf->value (SystemUIGConf::LedAllEnabled).toBool();
}

void
LedBusinessLogic::setLedsEnabled (
        bool enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_SystemUIGConf->setValue (SystemUIGConf::LedAllEnabled, enabled);
    /*
     * FIXME: Not implemented yet.
     */
}

bool
LedBusinessLogic::illuminationLedEnabled ()
{
    return m_SystemUIGConf->value (
            SystemUIGConf::LedIlluminationEnabled).toBool();
}

void
LedBusinessLogic::setIlluminationLedEnabled (
        bool enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_SystemUIGConf->setValue (SystemUIGConf::LedIlluminationEnabled, enabled);
    /*
     * FIXME: Not implemented yet.
     */
}

bool
LedBusinessLogic::eventsLedEnabled ()
{
    return m_SystemUIGConf->value (SystemUIGConf::LedEventsEnabled).toBool();
}

void
LedBusinessLogic::setEventsLedEnabled (
        bool enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_SystemUIGConf->setValue (SystemUIGConf::LedEventsEnabled, enabled);
    /*
     * FIXME: Not implemented yet.
     */
}
