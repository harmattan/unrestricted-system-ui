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

#include "ledbusinesslogicadaptor.h"
#include "ledbusinesslogic.h"

#undef DEBUG
#include "debug.h"

LedBusinessLogicAdaptor::LedBusinessLogicAdaptor (
            QObject          *parent, 
            LedBusinessLogic *ledLogic) :
    QDBusAbstractAdaptor (parent),
    m_LedLogic (ledLogic)
{
    SYS_DEBUG ("");

    connect (ledLogic, SIGNAL(ledsStateChanged(bool)),
            this, SIGNAL(ledsStateChanged (bool)));
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
LedBusinessLogicAdaptor::setEventsLedEnabled (
	int          mask,
        bool         enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_LedLogic->setEventsLedEnabled (mask, enabled);
}

bool
LedBusinessLogicAdaptor::LedsEnabled ()
{
    SYS_DEBUG ("");
    return m_LedLogic->ledsEnabled ();
}

int
LedBusinessLogicAdaptor::EventsLedEnabled ()
{
    SYS_DEBUG ("");
    return m_LedLogic->eventsLedEnabled ();
}

