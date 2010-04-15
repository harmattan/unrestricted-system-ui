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
#include "ut_statusareawindow.h"
#include <MApplication>
#include "statusareawindow.h"
#include "statusarea_stub.h"
#include "statusindicatormenuwindow_stub.h"

QPixmap *statusAreaPixmap = NULL;
bool Ut_StatusAreaWindow_Scene_Render_Called = false;
QRectF rectReceived(0,0,0,0);

const MStyle* MTheme::style(const char *styleClassName,
                             const QString &objectName,
                             const QString &mode,
                             const QString &type,
                             M::Orientation orientation,
                             const MWidgetController *parent)
{
    Q_UNUSED(styleClassName);
    Q_UNUSED(objectName);
    Q_UNUSED(mode);
    Q_UNUSED(type);
    Q_UNUSED(orientation);
    Q_UNUSED(parent);
    return NULL;
}

void QGraphicsScene::render(QPainter *painter, const QRectF &target, const QRectF &source, Qt::AspectRatioMode aspectRatioMode)
{
    Ut_StatusAreaWindow_Scene_Render_Called = true;
    rectReceived= source;
    Q_UNUSED(target);
    Q_UNUSED(painter);
    Q_UNUSED(aspectRatioMode);
}

void Ut_StatusAreaWindow::init()
{
    statusAreaWindow = new StatusAreaWindow();
    statusAreaPixmap = new QPixmap(30,80);
    statusAreaWindow->statusAreaPixmap = statusAreaPixmap;
}

void Ut_StatusAreaWindow::cleanup()
{
    Ut_StatusAreaWindow_Scene_Render_Called = false;
    rectReceived.setRect(0,0,0,0);
    delete statusAreaWindow;
}

void Ut_StatusAreaWindow::initTestCase()
{
    int argc = 1;
    char *app_name = (char *)"./Ut_StatusAreaWindow";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusAreaWindow::cleanupTestCase()
{
    delete app;
}

void Ut_StatusAreaWindow::testSceneChanged()
{
    // Test an intersected rect
    QList<QRectF> rectList;
    QRectF rect1(0,0,30,80);
    rectList.append(rect1);
    connect(this, SIGNAL(changed(QList<QRectF>)), statusAreaWindow, SLOT(sceneChanged(QList<QRectF>)));
    emit changed(rectList);
    QCOMPARE(Ut_StatusAreaWindow_Scene_Render_Called, true);

    // Test when rect has no intersection with pixmap
    Ut_StatusAreaWindow_Scene_Render_Called = false;
    rectList.pop_front();
    QRectF rect2(40,50,30,80);
    rectList.append(rect2);
    emit changed(rectList);
    QCOMPARE(Ut_StatusAreaWindow_Scene_Render_Called, false);

    // Test when two rects called then scene render called with complete intersection rect( final rect should be Rect1 U Rect2 Intersectin sharedPixmapRect)
    rectList.pop_front();
    QRectF rect3(0,0,10,20);
    QRectF rect4(10,20,10,20);
    rectList.append(rect3);
    rectList.append(rect4);
    emit changed(rectList);
    QCOMPARE(Ut_StatusAreaWindow_Scene_Render_Called, true);
    QRectF unitedRect = rect3.united(rect4) ;
    QRectF expectedRect = unitedRect.intersected(QRectF(0,0,30,80));
    QCOMPARE(rectReceived,expectedRect);
}

QTEST_APPLESS_MAIN(Ut_StatusAreaWindow)
