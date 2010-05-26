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

#ifndef LAYOUTREQUESTLISTENERWIDGET_STUB
#define LAYOUTREQUESTLISTENERWIDGET_STUB

#include "statusindicatormenuwindow.h"
#include <QGraphicsItem>
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LayoutRequestListenerWidgetStub : public StubBase {
public:
    virtual void LayoutRequestListenerWidgetConstructor(QGraphicsItem *parent);
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    virtual bool event(QEvent *event);
};

// 2. IMPLEMENT STUB
void LayoutRequestListenerWidgetStub::LayoutRequestListenerWidgetConstructor(QGraphicsItem *parent) {
    Q_UNUSED(parent);

}
QVariant LayoutRequestListenerWidgetStub::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    QList<ParameterBase*> params;
    params.append( new Parameter<QGraphicsItem::GraphicsItemChange >(change));
    params.append( new Parameter<const QVariant & >(value));
    stubMethodEntered("itemChange",params);
    return stubReturnValue<QVariant>("itemChange");
}

bool LayoutRequestListenerWidgetStub::event(QEvent *event) {
    QList<ParameterBase*> params;
    params.append( new Parameter<QEvent * >(event));
    stubMethodEntered("event",params);
    return stubReturnValue<bool>("event");
}

// 3. CREATE A STUB INSTANCE
LayoutRequestListenerWidgetStub gDefaultLayoutRequestListenerWidgetStub;
LayoutRequestListenerWidgetStub* gLayoutRequestListenerWidgetStub = &gDefaultLayoutRequestListenerWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LayoutRequestListenerWidget::LayoutRequestListenerWidget(QGraphicsItem *parent) {
    gLayoutRequestListenerWidgetStub->LayoutRequestListenerWidgetConstructor(parent);
}

QVariant LayoutRequestListenerWidget::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    return gLayoutRequestListenerWidgetStub->itemChange(change, value);
}

bool LayoutRequestListenerWidget::event(QEvent *event) {
    return gLayoutRequestListenerWidgetStub->event(event);
}


#endif
