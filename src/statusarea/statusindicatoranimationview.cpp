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

#include "statusindicatoranimationview.h"
#include "statusindicator.h"
#include <MViewCreator>
#include <MTheme>
#include <QGraphicsLinearLayout>
#include <QTimeLine>

#define DEBUG
#define WARNING
#include "debug.h"

StatusIndicatorAnimationView::StatusIndicatorAnimationView(StatusIndicator *controller) :
    MWidgetView(controller),
    controller(controller),
    animationFrame(0),
    firstAnimationFrame(0),
    animationTimeline(new QTimeLine(0, this))
{
    animationTimeline->setCurveShape(QTimeLine::LinearCurve);
    animationTimeline->setLoopCount(0);
    connect(animationTimeline, SIGNAL(frameChanged(int)), this, SLOT(setAnimationFrame(int)));
}

StatusIndicatorAnimationView::~StatusIndicatorAnimationView()
{
    clearImageList();
}

void StatusIndicatorAnimationView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char *> modifications;
    modifications << StatusIndicatorModel::Value;
    modifications << StatusIndicatorModel::Animate;
    updateData(modifications);
}

void StatusIndicatorAnimationView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == StatusIndicatorModel::Value) {
            if (model()->value().type() == QVariant::String) {
                setAnimationFrame(firstAnimationFrame);
                setupImageList(model()->value().toString());
           }
        } else if (member == BatteryStatusIndicatorModel::Animate) {
            if (model()->animate()) {
                startAnimation();
            } else {
                stopAnimation();
            }
        }
    }
}

void StatusIndicatorAnimationView::applyStyle()
{
    MWidgetView::applyStyle();

    setupAnimationTimeline();
    controller->update();
}

void StatusIndicatorAnimationView::setupAnimationTimeline()
{
    animationTimeline->setDuration(style()->animationDuration());
    animationTimeline->setFrameRange(firstAnimationFrame, images.size());
    animationTimeline->setUpdateInterval(style()->animationDuration() / (images.size() > 0 ? (images.size() - firstAnimationFrame) : 1));
}

void StatusIndicatorAnimationView::setFirstAnimationFrame(int frame)
{
    firstAnimationFrame = qBound(0, frame, images.size() - 1);
    setupAnimationTimeline();
}

void StatusIndicatorAnimationView::setAnimationFrame(int frame)
{
    frame = qBound(0, frame, images.size() - 1);

    if (animationFrame != frame) {
        animationFrame = frame;
        controller->update();
    }
}

void StatusIndicatorAnimationView::startAnimation()
{
    if (animationTimeline->state() == QTimeLine::NotRunning) {
        animationTimeline->start();
    }
}

void StatusIndicatorAnimationView::stopAnimation()
{
    if (animationTimeline->state() == QTimeLine::Running) {
        animationTimeline->stop();
    }
}

void StatusIndicatorAnimationView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    if (animationFrame < images.size() && size().width() > 0 && size().height() > 0) {
        if (images[animationFrame] == NULL) {
            // Load the image if it has not been loaded yet
            images[animationFrame] = MTheme::pixmapCopy(imageList.at(animationFrame));
        }

        if (images[animationFrame] != NULL) {
            // Paint the image
            painter->drawPixmap(QPointF(0, 0), *images[animationFrame]);
        }
    }
}

void StatusIndicatorAnimationView::clearImageList()
{
    for (int i = 0; i < images.size(); i++) {
        delete images[i];
    }

    imageList.clear();
    images.clear();

    animationFrame = 0;
}

void StatusIndicatorAnimationView::setupImageList(const QString &iconIDs)
{
    clearImageList();

    if (!iconIDs.isEmpty()) {
        // Use the "icon exists" style
        style().setModeIcon();

        // Create an icon list
        imageList = iconIDs.trimmed().split(QChar(' '));
        images = QVector<const QPixmap *>(imageList.length(), NULL);

        setupAnimationTimeline();
    } else {
        // Use the "no icon" (default) style
        style().setModeDefault();
    }

    // Redraw
    controller->updateGeometry();
    controller->update();
}

M_REGISTER_VIEW_NEW(StatusIndicatorAnimationView, StatusIndicator)
