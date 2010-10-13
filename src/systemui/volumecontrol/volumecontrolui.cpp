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
#include "volumecontrolui.h"
#include "volumeoverlay.h"
#include "volumebarlogic.h"

#include <QTimer>

#undef DEBUG
#include "../debug.h"

VolumeControlUI::VolumeControlUI (QObject *parent) :
    QObject (parent),
    m_logic (new VolumeBarLogic),
    m_overlay (0),
#ifdef HAVE_QMSYSTEM
    m_hwkeys (0),
#endif
    m_locked (false)
{
#ifdef HAVE_QMSYSTEM
    m_hwkeys = new Maemo::QmKeys (this);

    m_locks = new Maemo::QmLocks (this);

    connect (m_locks, SIGNAL (stateChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)),
             this, SLOT (locksChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
#endif

#ifdef HAVE_LIBRESOURCEQT
    m_hwkeyResource = new ResourcePolicy::ResourceSet ("event");
    m_hwkeyResource->setAlwaysReply ();

    ResourcePolicy::ScaleButtonResource *volumeKeys = new ResourcePolicy::ScaleButtonResource;

    m_hwkeyResource->addResourceObject (volumeKeys);

    connect (m_hwkeyResource, SIGNAL (resourcesGranted (QList<ResourcePolicy::ResourceType>)),
             this, SLOT (hwKeyResourceAcquired ()));
    connect (m_hwkeyResource, SIGNAL (lostResources ()),
             this, SLOT (hwKeyResourceLost ()));

    m_hwkeyResource->acquire ();
#endif
}

VolumeControlUI::~VolumeControlUI ()
{
#ifdef HAVE_LIBRESOURCEQT
    //Free the resources here
    m_hwkeyResource->deleteResource (ResourcePolicy::ScaleButtonType);
#endif

#ifdef HAVE_QMSYSTEM
    delete m_hwkeys;
    m_hwkeys = 0;

    delete m_locks;
    m_locks = 0;
#endif

    delete m_logic;
    m_logic = 0;

    delete m_overlay;
    m_overlay = 0;
}

void
VolumeControlUI::overlayChanged (int val)
{
    // Set the volume value
    m_logic->setVolume ((quint32) val);
}

#ifdef HAVE_QMSYSTEM
void
VolumeControlUI::hwKeyEvent (Maemo::QmKeys::Key key, Maemo::QmKeys::State state)
{
    int change_val = 0;

    if (state == Maemo::QmKeys::KeyUp)
        return;

    switch (key)
    {
        case Maemo::QmKeys::VolumeUp:
            SYS_DEBUG ("volume-up");
            change_val++;
            break;
        case Maemo::QmKeys::VolumeDown:
            SYS_DEBUG ("volume-down");
            change_val--;
            break;
        default:
            // no-op for other hw-keys...
            return;
            break;
    }

    /*
     * Re-query the actual values when connection lost
     * to PulseAudio daemon....
     */
    m_logic->ping ();

    int current_volume = (int) m_logic->getVolume ();
    int max_volume = (int) m_logic->getMaxVolume ();

    current_volume += change_val;

    if (current_volume >= max_volume)
        current_volume = max_volume - 1;
    if (current_volume < 0)
        current_volume = 0;

    // This sets the volume and update the slider ...
    overlayChanged (current_volume);

    /*
     * When screen/device is locked, do not show the overlay:
     */
    SYS_DEBUG ("Locked: %s", SYS_BOOL (m_locked));
    if (m_locked == true)
        return;

    if (m_overlay == 0)
    {
        m_overlay = new VolumeOverlay;

        connect (m_overlay, SIGNAL (VolumeChanged (int)),
                 this, SLOT (overlayChanged (int)));
    }
    // ... and show the overlay
    m_overlay->UpdateVolume (current_volume, max_volume);
}

void
VolumeControlUI::locksChanged (
    Maemo::QmLocks::Lock what, Maemo::QmLocks::State how)
{
    if (how == Maemo::QmLocks::Locked)
    {
        m_locked = true;
        if (m_overlay)
            m_overlay->hideWindow ();
    }
    else if (how == Maemo::QmLocks::Unlocked)
    {
        /*
         * Check wether all the locks went away...
         */
        if ((what == Maemo::QmLocks::Device) &&
            (m_locks->getState (Maemo::QmLocks::TouchAndKeyboard) ==
             Maemo::QmLocks::Unlocked))
        {
            m_locked = false;
        }
        else if (m_locks->getState (Maemo::QmLocks::Device) ==
                 Maemo::QmLocks::Unlocked)
        {
            m_locked = false;
        }
    }
}
#endif

void
VolumeControlUI::hwKeyResourceAcquired ()
{
    SYS_DEBUG ("");
#ifdef HAVE_QMSYSTEM
    // Disconnect from everything first
    m_hwkeys->disconnect ();

    connect(m_hwkeys, SIGNAL (keyEvent (Maemo::QmKeys::Key, Maemo::QmKeys::State)),
            this, SLOT (hwKeyEvent (Maemo::QmKeys::Key, Maemo::QmKeys::State)));
#endif
}

void
VolumeControlUI::hwKeyResourceLost ()
{
    SYS_DEBUG ("");
#ifdef HAVE_QMSYSTEM
    m_hwkeys->disconnect ();
#endif
}

