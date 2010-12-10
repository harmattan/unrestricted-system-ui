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

VolumeBarWindow::VolumeBarWindow(VolumeBarLogic *logic, QWidget *parent) :
    MWindow(parent),
    logic(logic),
    volumeBar(new VolumeBar),
    overlay(new MOverlay)
{
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
}

void VolumeBarWindow::volumeBarChanged(int val)
{
    logic->setVolume((quint32) val);
}

void VolumeBarWindow::updateVolume()
{
    volumeBar->updateVolume(logic->volume(), logic->maxVolume());
}
