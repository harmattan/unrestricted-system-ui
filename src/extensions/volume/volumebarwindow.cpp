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
    volumeBar(new VolumeBar)
{
    // Set the window attributes
    setSceneManager(new MSceneManager);
    setTranslucentBackground(true);
    setAttribute(Qt::WA_X11NetWmWindowTypeNotification, true);
    setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
    setObjectName("VolumeBarWindow");
    setProperty("followsCurrentApplicationWindowOrientation", true);

    // Set up the volume bar widget
    connect(volumeBar, SIGNAL(percentageChanged(qreal)), this, SLOT(setVolume(qreal)));
    connect(volumeBar, SIGNAL(animationsFinished()), this, SLOT(hide()));
    connect(this, SIGNAL(orientationChangeFinished(M::Orientation)), volumeBar, SLOT(updateContents()));

    // Overlay for the volume bar
    MOverlay *overlay = new MOverlay;
    overlay->setWidget(volumeBar);
    sceneManager()->appearSceneWindowNow(overlay);
}

VolumeBarWindow::~VolumeBarWindow()
{
}

void VolumeBarWindow::setVolume(qreal percentage)
{
    logic->setVolume(percentage * (logic->maxVolume() - 1));
}

void VolumeBarWindow::updateVolume()
{
    volumeBar->setTargetPercentage(logic->volume() / (qreal)(logic->maxVolume() - 1));
}
