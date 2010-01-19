/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "profilebrief.h"
#include "dcpwidgettypes.h"
#include "profiledatainterface.h"

ProfileBrief::ProfileBrief()
{
}

QString ProfileBrief::valueText() const
{
    ProfileDataInterface profileIf;
    return profileIf.getCurrentProfileName();
}


