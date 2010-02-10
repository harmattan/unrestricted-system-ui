/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaybrief.h"
#include "dcpwidgettypes.h"

/*
 * We don't want to anything at the second line unless we can show something
 * meaningful. Actually the UI spec does not specify what to show...
 */
#if 0
QString DisplayBrief::valueText() const
{
    return "Implemented";
}
#endif


