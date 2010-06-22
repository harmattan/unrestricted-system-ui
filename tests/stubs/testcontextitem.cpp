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

int TestContextItem::constructor_called = 0;
int TestContextItem::destructor_called = 0;
int TestContextItem::subscribe_called = 0;
int TestContextItem::unsubscribe_called = 0;

TestContextItem::TestContextItem()
{
    constructor_called++;
}

TestContextItem::~TestContextItem()
{
    destructor_called++;
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
    subscribe_called++;
}

void TestContextItem::unsubscribe() const
{
    unsubscribe_called++;
}

