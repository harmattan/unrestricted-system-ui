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
#include <DuiApplication>
#include <DuiTheme>
#include "statusareawindow.h"
#include "statusarea_stub.h"
#include "statusindicatormenuwindow_stub.h"

using namespace Maemo;

QPixmap *statusAreaPixmap = NULL;
bool Ut_StatusAreaWindow_Scene_Render_Called = false;
QRectF rectReceived(0,0,0,0);

const QPixmap *DuiTheme::pixmap(const QString &id, const QSize &size)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    return statusAreaPixmap;
}

void QGraphicsScene::render(QPainter *painter,
            const QRectF &target, const QRectF &source,
            Qt::AspectRatioMode aspectRatioMode)
{
    Ut_StatusAreaWindow_Scene_Render_Called = true;
    Q_UNUSED(painter);
    rectReceived= target;
    Q_UNUSED(source);
    Q_UNUSED(aspectRatioMode);
}

void Ut_StatusAreaWindow::init()
{
    statusAreaWindow = new StatusAreaWindow();
    statusAreaPixmap = new QPixmap(30,80); // Anything but 50,50 which is error case when pixmap not found
    statusAreaWindow->init();
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
    app = new DuiApplication(argc, &app_name);
}

void Ut_StatusAreaWindow::cleanupTestCase()
{
    Ut_StatusAreaWindow_Scene_Render_Called = false;
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
    QRectF expectedRect = unitedRect.intersected(QRectF(0,0,30,80)); // 0,0,30,80 come from the size of the pixmap
    QCOMPARE(rectReceived,expectedRect);
}

QList<QRectF>* setupRect()
{
    QList<QRectF>* rectList = new QList<QRectF>;
    QRectF rect1(0,0,30,80);
    rectList->append(rect1);
    return rectList;
}

QList<QRectF>* Ut_StatusAreaWindow::setupRenderTests()
{
    QList<QRectF>* rectList = setupRect();
    connect(this, SIGNAL(changed(QList<QRectF>)), statusAreaWindow, SLOT(sceneChanged(QList<QRectF>)));
    connect(this, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)), statusAreaWindow, SLOT(setSceneRender(Maemo::QmDisplayState::DisplayState)));
    return rectList;
}

void Ut_StatusAreaWindow::testSceneRenderControlDisplayStateOn()
{
    QList<QRectF>* rectList = setupRenderTests();
    emit displayStateChanged(Maemo::QmDisplayState::On);
    emit changed(*rectList);
    QCOMPARE(Ut_StatusAreaWindow_Scene_Render_Called, true);
   delete rectList;
}

void Ut_StatusAreaWindow::testSceneRenderControlDisplayStateOff()
{
    QList<QRectF>* rectList = setupRenderTests();
    emit displayStateChanged(Maemo::QmDisplayState::Off);
    emit changed(*rectList);
    QCOMPARE(Ut_StatusAreaWindow_Scene_Render_Called, false);
    delete rectList;
}

void Ut_StatusAreaWindow::testSceneRenderControlDisplayStateDimmed()
{
    QList<QRectF>* rectList = setupRenderTests();
    emit displayStateChanged(Maemo::QmDisplayState::Dimmed);
    emit changed(*rectList);
    QCOMPARE(Ut_StatusAreaWindow_Scene_Render_Called, false);
    delete rectList;
}

QTEST_APPLESS_MAIN(Ut_StatusAreaWindow)
