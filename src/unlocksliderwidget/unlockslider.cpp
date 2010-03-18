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
#include <QDebug>

#include "unlockslider.h"
#include "unlocksliderview.h"

UnlockSlider::UnlockSlider(QGraphicsItem *parent) :
        DuiWidgetController(new UnlockSliderModel(), parent)
{
    this->setView(view = new UnlockSliderView(this));
}

UnlockSlider::~UnlockSlider()
{
    view->deleteLater();
    view = NULL;
}

void UnlockSlider::hwKeyDown(bool down)
{
    qDebug() << "hwKeyDown " << down;
    //aHWKeyDown = down;
    if (!down) { //hw button released
        view->releaseHandle();
        emit released();
    }
}

bool UnlockSlider::isHandlePressed() const
{
    return model()->handlePressed();
}

const qreal& UnlockSlider::position() const
{
    return model()->position();
}

bool UnlockSlider::isIconVisible() const
{
    return model()->iconVisible();
}

void UnlockSlider::setIconVisible(bool iconVisible)
{
    model()->setIconVisible(iconVisible);
}

bool UnlockSlider::isMagnetic() const
{
    return model()->magnetic();
}

void UnlockSlider::setMagnetic(bool magnetic)
{
    model()->setMagnetic(magnetic);
}

void UnlockSlider::setOrientation(Qt::Orientation orientation)
{
    model()->setOrientation(orientation);
}

Qt::Orientation UnlockSlider::orientation() const
{
    return model()->orientation();
}

void UnlockSlider::setPosition(const qreal& position)
{
    model()->setPosition(position);
}

void UnlockSlider::reset()
{
    model()->setPosition(0);
}

void UnlockSlider::blink()
{
    model()->setBlinking(true);
}

void UnlockSlider::updateData(const QList<const char*>& modifications)
{
    DuiWidgetController::updateData(modifications);

    const char* member;
    foreach(member, modifications) {
        if (member == UnlockSliderModel::Position) {
            if (model()->position() > 0.99f) {
                emit unlocked();
            }
        }
        if (member == UnlockSliderModel::HandlePressed) {
            qDebug() << "released " << model()->handlePressed();
            if (!model()->handlePressed())
                emit released();
        }
    }
}
