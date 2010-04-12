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

QPixmap *statusAreaPixmap = NULL;
bool Ut_StatusAreaWindow_Scene_Render_Called = false;

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
    Q_UNUSED(target);
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
    QList<QRectF> rectList;
    QRectF rect(0,0,30,80);
    rectList.append(rect);
    connect(this, SIGNAL(changed(QList<QRectF>)), statusAreaWindow, SLOT(sceneChanged(QList<QRectF>)));
    emit changed(rectList);
    QCOMPARE(Ut_StatusAreaWindow_Scene_Render_Called, true);
}

QTEST_APPLESS_MAIN(Ut_StatusAreaWindow)
