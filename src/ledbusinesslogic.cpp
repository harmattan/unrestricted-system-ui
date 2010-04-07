/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "ledbusinesslogic.h"

#define DEBUG
#include "debug.h"

LedBusinessLogic::LedBusinessLogic (
        SystemUIGConf *systemUIGConf, 
        QObject       *parent) :
    QObject (parent),
    m_SystemUIGConf (systemUIGConf),
    m_Led (new QmLED)
{
    SYS_DEBUG ("");
    SYS_DEBUG ("*** int  eventsLedEnabled()");
    SYS_DEBUG ("*** bool ledsEnabled()");
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

/*!
 * Returns true if the led master switch is turned on.
 */
bool
LedBusinessLogic::ledsEnabled ()
{
    return m_SystemUIGConf->value (SystemUIGConf::LedAllEnabled).toBool();
}

/*
 * Sets the state of the master switch.
 */
void
LedBusinessLogic::setLedsEnabled (
        bool enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_SystemUIGConf->setValue (SystemUIGConf::LedAllEnabled, enabled);
    ensureLedStates ();
}

/*!
 * Returns information about the enabled led patterns. Every pattern is
 * represented by a bit in the returned integer.
 */
int
LedBusinessLogic::eventsLedEnabled ()
{
    int retval = 0;

    if (m_SystemUIGConf->value (SystemUIGConf::MissedCallLed).toBool())
        retval |= 1;
    if (m_SystemUIGConf->value (SystemUIGConf::SMSReceivedLed).toBool())
        retval |= 2;
    if (m_SystemUIGConf->value (SystemUIGConf::EmailReceivedLed).toBool())
        retval |= 4;
    if (m_SystemUIGConf->value (SystemUIGConf::InstantMessageReceivedLed).toBool())
        retval |= 8;
    if (m_SystemUIGConf->value (SystemUIGConf::ChargingLed).toBool())
        retval |= 16;
    if (m_SystemUIGConf->value (SystemUIGConf::OtherNotificationsLed).toBool())
        retval |= 32;

    SYS_DEBUG ("Returning %d", retval);
    return retval;
}

/*!
 * This method is used to set one or more specific led pattern on and off. The
 * mask specifies which led pattern(s) to toggle, the second parameter
 * determines if the pattern(s) should be enabled or disabled.
 */
void
LedBusinessLogic::setEventsLedEnabled (
        int  mask,
        bool enabled)
{
    Q_UNUSED (enabled);
    Q_UNUSED (mask);

    SYS_DEBUG ("*** mask    = %d", mask);
    SYS_DEBUG ("*** enabled = %s", SYS_BOOL(enabled));
    if (mask & 1)
        m_SystemUIGConf->setValue (SystemUIGConf::MissedCallLed, enabled);
    if (mask & 2)
        m_SystemUIGConf->setValue (SystemUIGConf::SMSReceivedLed, enabled);
    if (mask & 4)
        m_SystemUIGConf->setValue (SystemUIGConf::EmailReceivedLed, enabled);
    if (mask & 8)
        m_SystemUIGConf->setValue (SystemUIGConf::InstantMessageReceivedLed, enabled);
    if (mask & 16)
        m_SystemUIGConf->setValue (SystemUIGConf::ChargingLed, enabled);
    if (mask & 32)
        m_SystemUIGConf->setValue (SystemUIGConf::OtherNotificationsLed, enabled);

    ensureLedStates ();
}

/*!
 * This method is called whenever the led state has been changed. 
 */
void 
LedBusinessLogic::ensureLedStates ()
{
    bool allLedEnabled;

    allLedEnabled = m_SystemUIGConf->value (
            SystemUIGConf::LedAllEnabled).toBool();

    /*
     * Here we try to do some actual hw interaction, this is all we have.
     */
    if (allLedEnabled)
        m_Led->enable ();
    else 
        m_Led->disable ();
}

/*!
 * This slot is called when the GConf database has been modified. A signal will
 * be sent about the change.
 */
void 
LedBusinessLogic::gconfValueChanged (
        SystemUIGConf::GConfKey key, 
        QVariant                value)
{
    switch (key) {
        case SystemUIGConf::LedAllEnabled:
            emit ledsStateChanged(value.toBool());
            break;

        case SystemUIGConf::MissedCallLed:
        case SystemUIGConf::SMSReceivedLed:
        case SystemUIGConf::EmailReceivedLed:
        case SystemUIGConf::InstantMessageReceivedLed:
        case SystemUIGConf::ChargingLed:
        case SystemUIGConf::OtherNotificationsLed:
            /*
             * No matter which event led has been changed, we emit this signal.
             */
            emit eventsLedStateChanged(value.toBool());
            break;

        default:
            /*
             * This change is not about the event led.
             */
            break;
    }

    /* 
     * FIXME: Should not we call the ensureLedStates() method?
     */
}

