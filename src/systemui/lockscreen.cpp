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
#include "lockscreen.h"
#include <MWindow>

LockScreen::LockScreen(MWindow* parent)
{
    this->setParent(parent);
    setObjectName ("LockScreen");
}

LockScreen::~LockScreen()
{
}

void LockScreen::sliderUnlocked ()
{
    emit unlocked ();
}

void LockScreen::updateDateTime ()
{
    emit dateTimeChanged();
}

void LockScreen::reset ()
{
}
