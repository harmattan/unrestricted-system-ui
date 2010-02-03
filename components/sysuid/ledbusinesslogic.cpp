/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ledbusinesslogic.h"

//#define DEBUG
#include "../debug.h"

LedBusinessLogic::LedBusinessLogic (
        SystemUIGConf *systemUIGConf, 
        QObject       *parent) :
    QObject (parent),
    m_SystemUIGConf (systemUIGConf),
    m_Led (new QmLED)
{
    SYS_DEBUG ("");

    connect (
     m_SystemUIGConf, SIGNAL (valueChanged(SystemUIGConf::GConfKey, QVariant)),
     this, SLOT (gconfValueChanged (SystemUIGConf::GConfKey, QVariant)));

    /*
     * This is the class that stores the led states in the GConf database, so
     * here we read the configuration and set the hardware.
     */
    ensureLedStates ();
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
    ensureLedStates ();
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
    ensureLedStates ();
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
    ensureLedStates ();
}

void 
LedBusinessLogic::ensureLedStates ()
{
    bool allLedEnabled;
    bool eventsLedEnabled;
    bool illuminationLedEnabled;

    allLedEnabled = m_SystemUIGConf->value (
            SystemUIGConf::LedAllEnabled).toBool();
    illuminationLedEnabled = m_SystemUIGConf->value (
            SystemUIGConf::LedIlluminationEnabled).toBool();
    eventsLedEnabled = m_SystemUIGConf->value (
            SystemUIGConf::LedEventsEnabled).toBool();

    if (allLedEnabled && eventsLedEnabled)
        m_Led->enable ();
    else 
        m_Led->disable ();

    SYS_WARNING ("Don't know how to turn the illumination led %s",
        allLedEnabled && illuminationLedEnabled ? "on" : "off");
}

void 
LedBusinessLogic::gconfValueChanged (
        SystemUIGConf::GConfKey key, 
        QVariant                value)
{
    switch (key) {
        case SystemUIGConf::LedAllEnabled:
            emit ledsStateChanged(value.toBool());
            break;
        
        case SystemUIGConf::LedIlluminationEnabled:
            emit illuminationLedStateChanged(value.toBool());
            break;

        case SystemUIGConf::LedEventsEnabled:
            emit eventsLedStateChanged(value.toBool());
            break;
    }
}

