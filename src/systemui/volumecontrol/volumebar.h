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
#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include <QObject>
#include <MStylableWidget>
#include <QTimer>
#include "volumebarstyle.h"

class QPropertyAnimation;
class MStylableWidget;
class MImageWidget;
class MOverlay;
class MWindow;
class QTimer;

//! The volume-bar widget
class VolumeBar : public MStylableWidget
{
Q_OBJECT

public:
    //! Constructs a VolumeBar instance
    VolumeBar (QGraphicsItem *parent = 0);
    //! Destructs a VolumeBar instance
    ~VolumeBar ();

signals:
    /*! This signal emitted when user modifies the volume level by touching the volume-bar
     * \param val The new volume level
     */
    void volumeChanged (int val);
    //! this signal emitted when volume-bar runs to timeout
    void animationsFinished ();

public slots:
    /*! This method makes volume-bar visible, and updates its contents (with animations)
     * \param val The actual volume level (valid range: [0 - max-1])
     * \param max The maximal count of the volume-levels
     */
    void updateVolume (int val, int max);

private slots:
    //! This method updates the volume-bar contents
    void updateContents ();
    //! This method emits the done () signal and stops the timers
    void finishAnimations ();

protected:
    //! \reimp
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    virtual void applyStyle ();
    //! \reimp_end

private:
    //! a timer for visibility-timeout
    QTimer                   timer;
    //! the overlay main widget, the slider widget
    MStylableWidget         *slider;
    //! the volume-icon widget
    MImageWidget            *icon;
    //! the actual volume-level
    int                      value;
    //! the actual volume step-count
    int                      valueMax;
    //! animation for slider-movement
    QPropertyAnimation      *animMove;
    //! animation for fade-in effect
    QPropertyAnimation      *animFadeIn;
    //! animation for fade-out effect
    QPropertyAnimation      *animFadeOut;

private:
    Q_DISABLE_COPY(VolumeBar)
    M_STYLABLE_WIDGET(VolumeBarStyle)

    //! a private member which constructs the UI
    void constructUi ();

#ifdef UNIT_TEST
    friend class Ut_VolumeBar;
#endif
};

#endif
