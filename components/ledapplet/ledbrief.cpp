/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ledbrief.h"
#include "dcpwidgettypes.h"

#define DEBUG
#include "../debug.h"

int 
LedBrief::widgetTypeID () const
{
    return DcpWidgetType::Toggle;
}

bool
LedBrief::toggle () const
{
    SYS_DEBUG ("returning false");
    /*
     * Not implemented.
     */
    return false;
}

void
LedBrief::setToggle (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");
}

