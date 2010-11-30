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
#include <QDebug>

VolumeBarWindow::VolumeBarWindow(QWidget *parent) :
    MWindow(parent),
    logic(new VolumeBarLogic),
    volumeBar(new VolumeBar),
    overlay(new MOverlay)
#ifdef HAVE_QMSYSTEM
    ,hwkeys(new MeeGo::QmKeys(this))
#endif
{
#ifdef HAVE_LIBRESOURCEQT
    hwkeyResource = new ResourcePolicy::ResourceSet("event");
    hwkeyResource->setAlwaysReply();

    ResourcePolicy::ScaleButtonResource *volumeKeys = new ResourcePolicy::ScaleButtonResource;
    hwkeyResource->addResourceObject(volumeKeys);
    connect(hwkeyResource, SIGNAL(resourcesGranted(QList<ResourcePolicy::ResourceType>)), this, SLOT(hwKeyResourceAcquired()));
    connect(hwkeyResource, SIGNAL(lostResources()), this, SLOT(hwKeyResourceLost()));

    hwkeyResource->acquire();
#endif

    // Set up the volume bar widget
    connect(volumeBar, SIGNAL(volumeChanged(int)), SLOT(volumeBarChanged(int)));
    connect(volumeBar, SIGNAL(animationsFinished()), SLOT(hide()));
    connect(this, SIGNAL(orientationChangeFinished(M::Orientation)), volumeBar, SLOT(updateContents()));
    overlay->setWidget(volumeBar);
    sceneManager()->appearSceneWindowNow(overlay);

    // Set the window attributes
    setTranslucentBackground(true);
    setAttribute(Qt::WA_X11NetWmWindowTypeNotification, true);
    setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
    setObjectName("VolumeBarWindow");
    setProperty("followsCurrentApplicationWindowOrientation", true);
}

VolumeBarWindow::~VolumeBarWindow()
{
#ifdef HAVE_LIBRESOURCEQT
    //Free the resources here
    hwkeyResource->deleteResource(ResourcePolicy::ScaleButtonType);
#endif

    delete logic;
}

void VolumeBarWindow::volumeBarChanged(int val)
{
    // Set the volume value
    logic->setVolume((quint32) val);
}

#ifdef HAVE_QMSYSTEM
void VolumeBarWindow::hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    if (state == MeeGo::QmKeys::KeyUp) {
        // Do nothing on key releases
        return;
    }

    int volumeChange = 0;
    switch (key) {
    case MeeGo::QmKeys::VolumeUp:
        volumeChange = 1;
        break;
    case MeeGo::QmKeys::VolumeDown:
        volumeChange = -1;
        break;
    default:
        // no-op for other hw keys
        return;
    }

    int volume = (int)logic->volume() + volumeChange;
    int maxVolume = (int)logic->maxVolume();

    // Keep the volume within limits
    if (volume >= maxVolume) {
        volume = maxVolume - 1;
    } else if (volume < 0) {
        volume = 0;
    }

    // Take the new volume into use
    volumeBarChanged(volume);

    // Show the volume bar if the window is not visible
    if (!isVisible()) {
        show();
    }
    raise();

    // Update the volume bar geometry
    volumeBar->updateVolume(volume, maxVolume);
}
#endif

void VolumeBarWindow::hwKeyResourceAcquired()
{
#ifdef HAVE_QMSYSTEM
    // Disconnect from everything first
    hwkeys->disconnect ();

    connect(hwkeys, SIGNAL (keyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)), this, SLOT (hwKeyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)));
#endif
}

void VolumeBarWindow::hwKeyResourceLost()
{
#ifdef HAVE_QMSYSTEM
    hwkeys->disconnect();
#endif
}
