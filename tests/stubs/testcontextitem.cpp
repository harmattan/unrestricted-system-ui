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
#include "testcontextitem.h"

void ContextItemStub::ContextItemConstructor()
{
    stubMethodEntered("ContextItemConstructor");
}

void ContextItemStub::ContextItemDestructor()
{
    stubMethodEntered("ContextItemDestructor");
}

void ContextItemStub::subscribe()
{
    stubMethodEntered("subscribe");
}

void ContextItemStub::unsubscribe()
{
    stubMethodEntered("unsubscribe");
}

ContextItemStub gDefaultContextItemStub;
ContextItemStub *gContextItemStub = &gDefaultContextItemStub;

TestContextItem::TestContextItem()
{
    gContextItemStub->ContextItemConstructor();
}

TestContextItem::~TestContextItem()
{
    gContextItemStub->ContextItemDestructor();
}

void TestContextItem::setValue(const QVariant &val)
{
    value_ = val;
    emit contentsChanged();
}

QVariant TestContextItem::value() const
{
    return value_;
}

void TestContextItem::subscribe() const
{
    gContextItemStub->subscribe();
}

void TestContextItem::unsubscribe() const
{
    gContextItemStub->unsubscribe();
}

