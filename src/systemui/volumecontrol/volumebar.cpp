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
#include "volumebar.h"

#include <QTimer>
#include <QDebug>
#include <QRegion>
#include <QPointF>
#include <QSizeF>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>

#include <MSceneWindow>
#include <MImageWidget>
#include <MSceneManager>
#include <MLayout>
#include <MLinearLayoutPolicy>

VolumeBar::VolumeBar (QGraphicsItem *parent) :
    MStylableWidget (parent),
    slider (0),
    icon (0),
    value (0),
    valueMax (0),
    animMove (0),
    animFadeIn (0),
    animFadeOut (0)
{
    constructUi ();
    setOpacity (0.0);
}

VolumeBar::~VolumeBar ()
{
}

void VolumeBar::applyStyle ()
{
    MStylableWidget::applyStyle ();

    int timeout = style ()->visibleTimeout ();
    int animDuration = style ()->fadeDuration ();
    qreal opacity = style ()->volumebarOpacity ();

    if (animDuration * 2 > timeout)
    { // handle the error:
        qWarning () << "ERROR in VolumeBars CSS: animDuration * 2 > timeout!";
        animDuration = timeout / 2;
    }

    // first fade out animation started and after its finished we emit animationsFinished ()
    timer.setInterval (timeout - animDuration);

    animFadeIn->setDuration (animDuration);
    animFadeOut->setDuration (animDuration);

    animFadeIn->setEndValue (opacity);
}

void VolumeBar::constructUi ()
{
    slider = new MStylableWidget (this);
    slider->setObjectName ("FSVolumeBar");

    icon = new MImageWidget (this);

    MLayout *layout = new MLayout;
    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy (layout, Qt::Horizontal);
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    landscapePolicy->addStretch (2);
    landscapePolicy->addItem (icon);
    landscapePolicy->addStretch (1);

    portraitPolicy->addStretch (2);
    portraitPolicy->addItem (icon);
    portraitPolicy->addStretch (1);

    layout->setLandscapePolicy (landscapePolicy);
    layout->setPortraitPolicy (portraitPolicy);

    // prepare the fade in & out animations...
    animFadeIn = new QPropertyAnimation (this, "opacity");
    animFadeIn->setStartValue (0.0);

    animFadeOut = new QPropertyAnimation (this, "opacity");
    animFadeOut->setEndValue (0.0);

    connect (&timer, SIGNAL(timeout()),
             animFadeOut, SLOT(start()));
    connect (animFadeOut, SIGNAL(finished()), SLOT(finishAnimations()));

    // .. and prepare the movement animation also
    animMove = new QPropertyAnimation (slider, "geometry");

    setLayout (layout);
}

void VolumeBar::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
  MStylableWidget::mouseMoveEvent (event);
  // just pass the event to mousePressEvent handler
  mousePressEvent (event);
}

void VolumeBar::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    MStylableWidget::mousePressEvent (event);
    qreal percentage = 0.0;
    /*
     * increase & decrease the volume based on the tap/movement
     * coordinates...
     */
    if (sceneManager()->orientation() == M::Portrait)
        percentage = (geometry().height() - event->pos().y()) / geometry().height();
    else
        percentage = (geometry().width() - event->pos().x()) / geometry().width();

    value = valueMax * percentage;

    if (value >= valueMax)
        value = valueMax - 1;

    /* emit volumeChanged signal... */
    emit volumeChanged (value);
    /* and the update the UI */
    updateVolume (value, valueMax);
}

void VolumeBar::updateContents ()
{
    MSceneManager *sm = sceneManager ();
    QSize screen = sm->visibleSceneSize (sm->orientation ());

    QSizeF newSize;
    QRectF barGeom;

    QString iconID = (value == 0) ?
                     style ()->iconVolumeOff () :
                     style ()->iconVolume ();
    if (icon->imageId () != iconID)
        icon->setImage (iconID, style ()->iconVolumeSize ());

    /*
     * update the bar geometry based on current screen size,
     * and on actual volume-level
     */
    if (sm->orientation () == M::Portrait) {
        newSize.setHeight (screen.height () * value / (valueMax - 1));
        newSize.setWidth (screen.width ());

        barGeom.setX (0.0);
        barGeom.setY (screen.height () - newSize.height ());
    } else {
        newSize.setHeight (screen.height ());
        newSize.setWidth (screen.width () * value / (valueMax - 1));

        barGeom.setX (screen.width () - newSize.width ());
        barGeom.setY (0.0);
    }
    barGeom.setSize (newSize);

    if (isOnDisplay () == false)
    {
        // do not animate unnecessarily when we're not on display
        slider->setGeometry (barGeom);
    }
    else
    {
        animMove->setStartValue (slider->geometry ());
        animMove->setEndValue (barGeom);

        animMove->start ();
    }
}

void VolumeBar::updateVolume (int val, int max)
{
    timer.stop ();

    value = val;
    valueMax = max;

    updateContents ();

    if (animFadeOut->state () != QAbstractAnimation::Stopped) {
        // stop the fade-out animation in case if it is not stopped
        animFadeOut->stop ();
        // start the fade-in animation to get the wanted opacity back
        animFadeIn->start ();
    } else if (animFadeIn->state () != QAbstractAnimation::Running && (opacity () <= 0.1)) {
        // start the fade-in animation (only if needed ^^^)
        animFadeIn->start ();
    }

    timer.start ();
}

void VolumeBar::finishAnimations ()
{
    emit animationsFinished ();

    timer.stop ();
    animFadeIn->stop ();
    animFadeOut->stop ();

    setOpacity (0.0);
}

