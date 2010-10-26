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

#ifndef LOCKSCREENWINDOWSTYLE_H
#define LOCKSCREENWINDOWSTYLE_H

#include <MStyle>

class LockScreenWindowStyle : public MStyle
{
    Q_OBJECT
    M_STYLE(LockScreenWindowStyle)

    // The locked orientation: "landscape", "portrait" or any other value for unlocked
    M_STYLE_ATTRIBUTE(QString, lockedOrientation, LockedOrientation)
};

class LockScreenWindowStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER(LockScreenWindowStyle)
};

#endif
