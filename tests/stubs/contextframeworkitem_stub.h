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
#ifndef CONTEXTFRAMEWORKITEM_STUB
#define CONTEXTFRAMEWORKITEM_STUB

// A kludgy way to stub ContextFrameworkItem so that a ContextProperty
// member is not included since the latter does not have a default
// constructor defined.
#ifdef HAVE_CONTEXTSUBSCRIBER
#undef HAVE_CONTEXTSUBSCRIBER
#endif

#include "contextframeworkcontext.h"
#include <stubbase.h>


// 1. DECLARE STUB
class ContextFrameworkItemStub : public StubBase {
public:
    virtual void ContextFrameworkItemConstructor(const QString& key);
    virtual QVariant value();
    virtual void subscribe();
    virtual void unsubscribe();
};


// 2. IMPLEMENT STUB
void ContextFrameworkItemStub::ContextFrameworkItemConstructor(const QString& key)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<QString>(key));
    stubMethodEntered("ContextFrameworkItemConstructor", params);
}

QVariant ContextFrameworkItemStub::value()
{
    stubMethodEntered("value");
    return stubReturnValue<QVariant>("value");
}

void ContextFrameworkItemStub::subscribe()
{
    stubMethodEntered("subscribe");
}

void ContextFrameworkItemStub::unsubscribe()
{
    stubMethodEntered("unsubscribe");
}


// 3. CREATE A STUB INSTANCE
ContextFrameworkItemStub gDefaultContextFrameworkItemStub;
ContextFrameworkItemStub *gContextFrameworkItemStub = &gDefaultContextFrameworkItemStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ContextFrameworkItem::ContextFrameworkItem(const QString& key)
{
    gContextFrameworkItemStub->ContextFrameworkItemConstructor(key);
}

QVariant ContextFrameworkItem::value() const
{
    return gContextFrameworkItemStub->value();
}

void ContextFrameworkItem::subscribe() const
{
    return gContextFrameworkItemStub->subscribe();
}

void ContextFrameworkItem::unsubscribe() const
{
    return gContextFrameworkItemStub->unsubscribe();
}

#endif
