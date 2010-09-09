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
#include <unlockwidgets.h>

UnlockHeader::UnlockHeader() {

}

UnlockHeader::~UnlockHeader() {

}

void
UnlockHeader::updateDateTime() {

}

#ifdef HAVE_QMSYSTEM
void
UnlockHeader::timeSettingsChanged (Maemo::QmTimeWhatChanged what) {
    Q_UNUSED (what);
}
#endif

void
UnlockHeader::setActive (bool active)
{
    Q_UNUSED(active);
}

UnlockArea::UnlockArea() {

}

UnlockArea::~UnlockArea() {

}

void
UnlockArea::setEnabled (bool enabled)
{
    Q_UNUSED(enabled);
}

void
UnlockArea::setActive (bool active)
{
    Q_UNUSED(active);
}

