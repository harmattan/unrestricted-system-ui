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

#include <MStylableWidget>
#include <QTimer>
#include "volumebarstyle.h"

class QPropertyAnimation;
class MStylableWidget;
class MImageWidget;
class MOverlay;
class MWindow;
class QTimer;

/*!
 * The volume bar widget draws a background image, a volume bar image and a
 * volume indicator icon.
 */
class VolumeBar : public MStylableWidget {
    Q_OBJECT

    //! Current volume percentage
    Q_PROPERTY(qreal currentPercentage READ currentPercentage WRITE setCurrentPercentage)

public:
    //! Constructs a VolumeBar instance
    VolumeBar(QGraphicsItem *parent = NULL);

    //! Destroys the VolumeBar instance
    ~VolumeBar();

signals:
    /*!
     * Emitted when the user modifies the volume level by touching the volume bar
     *
     * \param percentage The new volume percentage
     */
    void percentageChanged(qreal percentage);

    //! Emitted when volume bar has faded out
    void animationsFinished();

public slots:
    /*!
     * Sets the target volume percentage of the volume bar
     *
     * \param percentage the target volume percentage
     */
    void setTargetPercentage(qreal percentage);

private slots:
    //! Updates the contents (icon and bar position) of the volume bar
    void updateContents();

    //! Emits the animationsFinished() signal and stops the timers
    void finishAnimations();

protected:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void applyStyle();
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

private:
    //! Calculate new volume based on the given mouse event
    void calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event);

    //! Returns the current volume percentage
    qreal currentPercentage() const;

    //! Sets the current volume percentage
    void setCurrentPercentage(qreal percentage);

    //! A timer for making the volume bar invisible after a timeout
    QTimer visibilityTimer;
    //! The volume icon widget
    MImageWidget *icon;
    //! Current volume percentage
    qreal currentPercentage_;
    //! Target volume percentage
    qreal targetPercentage;
    //! Percentage animation
    QPropertyAnimation *percentageAnimation;
    //! Fade in animation
    QPropertyAnimation *fadeInAnimation;
    //! Fade out animation
    QPropertyAnimation *fadeOutAnimation;

private:
    Q_DISABLE_COPY(VolumeBar)
    M_STYLABLE_WIDGET(VolumeBarStyle)

#ifdef UNIT_TEST
    friend class Ut_VolumeBar;
#endif
};

#endif
