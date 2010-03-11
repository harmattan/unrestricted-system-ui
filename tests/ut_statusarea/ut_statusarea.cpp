
/***************************************************************************
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
#include "ut_statusarea.h"
#include "statusarea.h"
#include "contextframeworkcontext.h"
#include <QGraphicsSceneMouseEvent>
#include "statusindicatormenuwindow.h"
// ContextFrameworkContext stubs (used by StatusArea)
QVariant ContextFrameworkItem::value() const
{
    return QVariant();
}

const QPointF mouseDownPos = QPointF(12.24, 20.20); // Arbitary point
const QPointF mouseMovePos = QPointF(15.20, 51.10); // Point with y greater than mouseDownPos + SWIPE_THRESHOLD
const QPointF badMouseMovePos = QPointF(15.20, 49.10); // Point with y less than mouseDownPos + SWIPE_THRESHOLD
bool mouseDown = false;
bool mouseMove = false;
bool badMouseMove = false;
QPointF QGraphicsSceneMouseEvent::pos() const
{
    if (mouseDown)
        return mouseDownPos;
    else if (mouseMove)
        return mouseMovePos;
    else if (badMouseMove)
        return badMouseMovePos;
    else
        return QPointF();
}

void Ut_StatusArea::init()
{
    statusArea = new StatusArea();
}

void Ut_StatusArea::cleanup()
{
    delete statusArea;
    mouseDown = false;
    mouseMove = false;
    badMouseMove = false;
}

void Ut_StatusArea::initTestCase()
{
}

void Ut_StatusArea::cleanupTestCase()
{
}

void Ut_StatusArea::testWhenSwipeStatusAreaStatusIndicatorMenuAppears()
{
    QGraphicsSceneMouseEvent mouseDownEvent(QEvent::GraphicsSceneMousePress);
    mouseDown = true;
    statusArea->sceneEvent(&mouseDownEvent);
    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    mouseMove = true;
    mouseDown = false;
    statusArea->sceneEvent(&mouseMoveEvent);
    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    statusArea->sceneEvent(&mouseReleaseEvent);
    QVERIFY(statusArea->statusIndicatorMenuWindow->isVisible());
}

void Ut_StatusArea::testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear()
{
    QGraphicsSceneMouseEvent mouseDownEvent(QEvent::GraphicsSceneMousePress);
    mouseDown = true;
    statusArea->sceneEvent(&mouseDownEvent);
    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    badMouseMove = true;
    mouseDown = false;
    statusArea->sceneEvent(&mouseMoveEvent);
    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    statusArea->sceneEvent(&mouseReleaseEvent);
    QVERIFY(!statusArea->statusIndicatorMenuWindow->isVisible());
}

QTEST_MAIN(Ut_StatusArea)
