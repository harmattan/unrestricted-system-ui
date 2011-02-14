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
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <MSceneWindow>
#include <MImageWidget>
#include <MSceneManager>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MScalableImage>

VolumeBar::VolumeBar(QGraphicsItem *parent) :
    MStylableWidget(parent),
    icon(new MImageWidget(this)),
    currentPercentage_(0),
    targetPercentage(0),
    percentageAnimation(new QPropertyAnimation(this, "currentPercentage")),
    fadeInAnimation(new QPropertyAnimation(this, "opacity")),
    fadeOutAnimation(new QPropertyAnimation(this, "opacity"))
{
    setOpacity(0);
    icon->setStyleName("VolumeBarIcon");

    // Set up the layout
    MLayout *layout = new MLayout;
    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    landscapePolicy->addStretch();
    landscapePolicy->addItem(icon);
    portraitPolicy->addStretch();
    portraitPolicy->addItem(icon);
    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);
    setLayout(layout);

    // Set up the animations
    fadeInAnimation->setStartValue(0);
    fadeInAnimation->setEndValue(1);
    fadeOutAnimation->setEndValue(0);
    connect(&visibilityTimer, SIGNAL(timeout()), fadeOutAnimation, SLOT(start()));
    connect(fadeOutAnimation, SIGNAL(finished()), this, SLOT(finishAnimations()));
}

VolumeBar::~VolumeBar()
{
}

void VolumeBar::applyStyle()
{
    MStylableWidget::applyStyle();

    int fadeDuration = style()->fadeDuration();
    fadeInAnimation->setDuration(fadeDuration);
    fadeOutAnimation->setDuration(fadeDuration);
    visibilityTimer.setInterval(style()->visibleDuration());
}

void VolumeBar::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    QSizeF currentSize = size();

    if (currentSize.width() != 0 && currentSize.height() != 0) {
        if (style()->backgroundColor().isValid()) {
            // Fill the background with the background opacity
            painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());
            painter->fillRect(boundingRect(), QBrush(style()->backgroundColor()));
        }

        if (style()->backgroundImage() != NULL) {
            // Draw the volume bar with the volume bar opacity
            qreal topHeight = (1 - currentPercentage_) * currentSize.height();
            qreal bottomHeight = currentPercentage_ * currentSize.height();
            painter->setOpacity(style()->volumeBarOpacity() * effectiveOpacity());
            style()->backgroundImage()->draw((qreal)0, topHeight, currentSize.width(), bottomHeight, painter);
        }
    }
}

void VolumeBar::calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event)
{
    // Calculate the new percentage based on the click coordinate
    setTargetPercentage((geometry().height() - event->pos().y()) / geometry().height());
    emit percentageChanged(targetPercentage);
}

void VolumeBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MStylableWidget::mousePressEvent(event);
    event->accept();
    calculateNewVolumeForEvent(event);
}

void VolumeBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    MStylableWidget::mouseMoveEvent(event);
    calculateNewVolumeForEvent(event);
}

void VolumeBar::updateContents()
{
    QString iconID = (targetPercentage == 0) ? style()->mutedIconId() : style()->iconId();
    if (icon->imageId() != iconID) {
        icon->setImage(iconID, style()->iconSize());
    }

    // Set the new percentage value
    if (!isOnDisplay()) {
        // Do not animate unnecessarily when we're not on display
        percentageAnimation->stop();
        currentPercentage_ = targetPercentage;
        update();
    } else {
        percentageAnimation->setStartValue(currentPercentage_);
        percentageAnimation->setEndValue(targetPercentage);
        percentageAnimation->start();
    }
}

void VolumeBar::setTargetPercentage(qreal percentage)
{
    visibilityTimer.stop();

    targetPercentage = percentage;

    updateContents();

    if (fadeOutAnimation->state() != QAbstractAnimation::Stopped) {
        // Stop the fade out animation in case if it is not stopped
        fadeOutAnimation->stop();
        // Start the fade in animation towards the target opacity
        fadeInAnimation->start();
    } else if (fadeInAnimation->state() != QAbstractAnimation::Running && (opacity() <= 0.1)) {
        // start the fade-in animation (only if needed ^^^)
        fadeInAnimation->start();
    }

    visibilityTimer.start();
}

void VolumeBar::finishAnimations()
{
    visibilityTimer.stop();
    fadeInAnimation->stop();
    fadeOutAnimation->stop();

    setOpacity(0);

    emit animationsFinished();
}

qreal VolumeBar::currentPercentage() const
{
    return currentPercentage_;
}

void VolumeBar::setCurrentPercentage(qreal percentage)
{
    currentPercentage_ = percentage;
    update();
}
