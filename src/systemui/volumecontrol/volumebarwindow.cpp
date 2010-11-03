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
#include "volumebarwindow.h"
#include "volumebar.h"
#include "volumebarlogic.h"

#include <MSceneManager>
#include <MOverlay>

VolumeBarWindow::VolumeBarWindow (QWidget *parent) :
    MWindow (parent),
    logic (new VolumeBarLogic),
    volumeBar (0),
    overlay (0),
#ifdef HAVE_QMSYSTEM
    hwkeys (0),
#endif
    locked (false)
{
#ifdef HAVE_QMSYSTEM
    hwkeys = new MeeGo::QmKeys (this);
    locks = new MeeGo::QmLocks (this);

    connect (locks, SIGNAL (stateChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)),
             SLOT (locksChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)));
#endif

#ifdef HAVE_LIBRESOURCEQT
    hwkeyResource = new ResourcePolicy::ResourceSet ("event");
    hwkeyResource->setAlwaysReply ();

    ResourcePolicy::ScaleButtonResource *volumeKeys = new ResourcePolicy::ScaleButtonResource;

    hwkeyResource->addResourceObject (volumeKeys);

    connect (hwkeyResource, SIGNAL (resourcesGranted (QList<ResourcePolicy::ResourceType>)),
             this, SLOT (hwKeyResourceAcquired ()));
    connect (hwkeyResource, SIGNAL (lostResources ()),
             this, SLOT (hwKeyResourceLost ()));

    hwkeyResource->acquire ();
#endif

    // create the VolumeBar widget
    volumeBar = new VolumeBar;
    connect (volumeBar, SIGNAL (volumeChanged (int)), SLOT (volumeBarChanged (int)));
    connect (volumeBar, SIGNAL (animationsFinished ()), SLOT (hide ()));

    connect (this, SIGNAL (orientationChangeFinished(M::Orientation)),
             volumeBar, SLOT (updateContents ()));

    // create&set-up an overlay
    overlay = new MOverlay;
    overlay->setWidget (volumeBar);

    // create a scenemanager and show the overlay
    setSceneManager (new MSceneManager);
    sceneManager ()->appearSceneWindow (overlay);

    // and set the window attributes
    setTranslucentBackground (true);
    setAttribute (Qt::WA_X11NetWmWindowTypeNotification, true);
    setAttribute (Qt::WA_X11DoNotAcceptFocus, true);
    setObjectName ("VolumeBarWindow");
    setProperty("followsCurrentApplicationWindowOrientation", true);
}

VolumeBarWindow::~VolumeBarWindow ()
{
#ifdef HAVE_LIBRESOURCEQT
    //Free the resources here
    hwkeyResource->deleteResource (ResourcePolicy::ScaleButtonType);
#endif

    delete logic;
    delete overlay;
}

void VolumeBarWindow::volumeBarChanged (int val)
{
    // Set the volume value
    logic->setVolume ((quint32) val);
}

#ifdef HAVE_QMSYSTEM
void VolumeBarWindow::hwKeyEvent (MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    int change_val = 0;

    if (state == MeeGo::QmKeys::KeyUp)
        return;

    switch (key)
    {
        case MeeGo::QmKeys::VolumeUp:
            change_val++;
            break;
        case MeeGo::QmKeys::VolumeDown:
            change_val--;
            break;
        default:
            // no-op for other hw-keys...
            return;
            break;
    }

    int current_volume = (int) logic->volume ();
    int max_volume = (int) logic->maxVolume ();

    current_volume += change_val;

    if (current_volume >= max_volume)
        current_volume = max_volume - 1;
    if (current_volume < 0)
        current_volume = 0;

    // This sets the volume
    volumeBarChanged (current_volume);

    // When screen/device is locked, do not show the volumeBar:
    if (locked == true)
        return;

    if (isVisible () == false)
        show ();

    // And update the VolumeBar widget contents
    volumeBar->updateVolume (current_volume, max_volume);
}

void VolumeBarWindow::locksChanged (MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how)
{
    if (how == MeeGo::QmLocks::Locked)
    {
        locked = true;
        // hide the window if it is visible
        if (isVisible () == true)
            hide ();
    }
    else if (how == MeeGo::QmLocks::Unlocked)
    {
        /*
         * Check wether all the locks went away...
         */
        if ((what == MeeGo::QmLocks::Device) &&
            (locks->getState (MeeGo::QmLocks::TouchAndKeyboard) ==
             MeeGo::QmLocks::Unlocked))
        {
            locked = false;
        }
        else if (locks->getState (MeeGo::QmLocks::Device) ==
                 MeeGo::QmLocks::Unlocked)
        {
            locked = false;
        }
    }
}
#endif

void VolumeBarWindow::hwKeyResourceAcquired ()
{
#ifdef HAVE_QMSYSTEM
    // Disconnect from everything first
    hwkeys->disconnect ();

    connect(hwkeys, SIGNAL (keyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)),
            this, SLOT (hwKeyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)));
#endif
}

void VolumeBarWindow::hwKeyResourceLost ()
{
#ifdef HAVE_QMSYSTEM
    hwkeys->disconnect ();
#endif
}

