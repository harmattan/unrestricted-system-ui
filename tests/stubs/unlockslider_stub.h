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
#ifndef UNLOCKSLIDER_STUB_H
#define UNLOCKSLIDER_STUB_H

#include <stubbase.h>
#include <unlockslider.h>

class UnlockSliderStub : public StubBase
{
public:
    virtual void UnlockSliderStubConstructor(QGraphicsItem *parent = NULL);
    virtual void UnlockSliderStubDestructor();
    virtual void hwKeyDown(bool down);
    virtual bool isHandlePressed() const;
    virtual const qreal& position() const;
    virtual bool isIconVisible() const;
    virtual void setIconVisible(bool);
    virtual bool isMagnetic() const;
    virtual void setMagnetic(bool);
    virtual void setOrientation(Qt::Orientation);
    virtual Qt::Orientation orientation() const;
    virtual void reset();
    virtual void setPosition(const qreal& position);
    virtual void blink();
};

void UnlockSliderStub::UnlockSliderStubConstructor(QGraphicsItem *parent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsItem *>(parent));
    stubMethodEntered("UnlockSliderStubConstructor", params);
}

void UnlockSliderStub::UnlockSliderStubDestructor()
{
    stubMethodEntered("UnlockSliderStubDestructor");
}

void UnlockSliderStub::hwKeyDown(bool down)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(down));
    stubMethodEntered("hwKeyDown", params);
}

bool UnlockSliderStub::isHandlePressed() const
{
    stubMethodEntered("isHandlePressed");
    return stubReturnValue<bool>("isHandlePressed");
}

const qreal& UnlockSliderStub::position() const
{
    stubMethodEntered("position");
    return stubReturnValue<qreal>("position");
}

bool UnlockSliderStub::isIconVisible() const
{
    stubMethodEntered("isIconVisible");
    return stubReturnValue<bool>("isIconVisible");
}

void UnlockSliderStub::setIconVisible(bool visible)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(visible));
    stubMethodEntered("setIconVisible", params);
}

bool UnlockSliderStub::isMagnetic() const
{
    stubMethodEntered("isMagnetic");
    return stubReturnValue<bool>("isMagnetic");
}

void UnlockSliderStub::setMagnetic(bool magnetic)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(magnetic));
    stubMethodEntered("setMagnetic", params);
}

void UnlockSliderStub::setOrientation(Qt::Orientation orientation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::Orientation>(orientation));
    stubMethodEntered("setOrientation", params);
}

Qt::Orientation UnlockSliderStub::orientation() const
{
    stubMethodEntered("orientation");
    return stubReturnValue<Qt::Orientation>("orientation");
}

void UnlockSliderStub::reset()
{
    stubMethodEntered("reset");
}

void UnlockSliderStub::setPosition(const qreal& position)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal>(position));
    stubMethodEntered("setPosition", params);
}

void UnlockSliderStub::blink()
{
    stubMethodEntered("blink");
}

UnlockSliderStub gDefaultUnlockSliderStub;
UnlockSliderStub *gUnlockSliderStub = &gDefaultUnlockSliderStub;

UnlockSlider::UnlockSlider(QGraphicsItem *parent)
{
    gUnlockSliderStub->UnlockSliderStubConstructor(parent);
}

UnlockSlider::~UnlockSlider()
{
    gUnlockSliderStub->UnlockSliderStubDestructor();
}

void UnlockSlider::hwKeyDown(bool down)
{
    gUnlockSliderStub->hwKeyDown(down);
}

bool UnlockSlider::isHandlePressed() const
{
    return gUnlockSliderStub->isHandlePressed();
}

const qreal& UnlockSlider::position() const
{
    return gUnlockSliderStub->position();
}

bool UnlockSlider::isIconVisible() const
{
    return gUnlockSliderStub->isIconVisible();
}

void UnlockSlider::setIconVisible(bool iconVisible)
{
    gUnlockSliderStub->setIconVisible(iconVisible);
}

bool UnlockSlider::isMagnetic() const
{
    return gUnlockSliderStub->isMagnetic();
}

void UnlockSlider::setMagnetic(bool magnetic)
{
    gUnlockSliderStub->setMagnetic(magnetic);
}

void UnlockSlider::setOrientation(Qt::Orientation orientation)
{
    gUnlockSliderStub->setOrientation(orientation);
}

Qt::Orientation UnlockSlider::orientation() const
{
    return gUnlockSliderStub->orientation();
}

void UnlockSlider::setPosition(const qreal& position)
{
    gUnlockSliderStub->setPosition(position);
}

void UnlockSlider::reset()
{
    gUnlockSliderStub->reset();
}

void UnlockSlider::blink()
{
    gUnlockSliderStub->blink();
}

void UnlockSlider::updateData(const QList<const char*>&)
{
}

#endif
