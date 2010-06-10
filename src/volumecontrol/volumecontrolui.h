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
#ifndef VOLUMECONTROLUI_H
#define VOLUMECONTROLUI_H

#include <QObject>

class VolumeBarLogic;
class VolumeOverlay;

#ifdef HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
#endif

#ifdef HAVE_QMSYSTEM
// For Hw-volume key handling
#include <qmkeys.h>

using namespace Maemo;
#endif

class VolumeControlUI : public QObject
{
    Q_OBJECT

public:
    VolumeControlUI (QObject *parent = 0);
    ~VolumeControlUI ();

private slots:
    void overlayChanged (int val);
#ifdef HAVE_QMSYSTEM
    void hwKeyEvent (QmKeys::Key key, QmKeys::State state);
#endif
    void hwKeyResourceAcquired ();
    void hwKeyResourceLost ();

private:
    VolumeBarLogic  *m_logic;
    VolumeOverlay   *m_overlay;

#ifdef HAVE_QMSYSTEM
    Maemo::QmKeys   *m_hwkeys;
#endif

#ifdef HAVE_LIBRESOURCEQT
    ResourcePolicy::ResourceSet *m_hwkeyResource;
#endif
};

#endif
