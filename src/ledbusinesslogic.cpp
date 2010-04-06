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
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ledbusinesslogic.h"

//#define DEBUG
#include "debug.h"

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

    allLedEnabled = m_SystemUIGConf->value (
            SystemUIGConf::LedAllEnabled).toBool();
    eventsLedEnabled = m_SystemUIGConf->value (
            SystemUIGConf::LedEventsEnabled).toBool();

    if (allLedEnabled && eventsLedEnabled)
        m_Led->enable ();
    else 
        m_Led->disable ();
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
        
        case SystemUIGConf::LedEventsEnabled:
            emit eventsLedStateChanged(value.toBool());
            break;

        default:
            break;
    }
}

