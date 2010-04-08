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

#include <DuiOrientationChangeEvent>
#include <DuiOnDisplayChangeEvent>
#include <DuiDeviceProfile>
#include <DuiTheme>
#include "statusareawindow.h"
#include "statusarea.h"

QString StatusAreaWindow::STATUS_AREA_PIXMAP_LOGICAL_ID = "duinavigationbar-statusarea-background";

StatusAreaWindow::StatusAreaWindow(QWidget *parent) :
    DuiWindow(NULL, parent),
    scene(new QGraphicsScene()),
    statusArea_(new StatusArea(NULL,this)),
    statusAreaPixmap(NULL)
{
    // Set the window type to _NET_WM_WINDOW_TYPE_DOCK
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);

    // Rotate to current orientation
    rotate(this->orientationAngle());
    connect(this, SIGNAL(orientationAngleChanged(const Dui::OrientationAngle &)), this, SLOT(rotate(const Dui::OrientationAngle &)));
}

void StatusAreaWindow::initializeScene()
{
    // Set the scene and add the status area to the scene
    setScene(scene);
    scene->addItem(statusArea_);
    // Get signaled when the scene changes
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(sceneChanged(QList<QRectF>)));
}

bool StatusAreaWindow::init()
{
    statusAreaPixmap = const_cast<QPixmap*>(DuiTheme::pixmap(STATUS_AREA_PIXMAP_LOGICAL_ID));
    QSize size = statusAreaPixmap->size();
    if(size == QSize(50,50)) {
        // When no pixmap is found then the theme daemon returns a pixmap of 50,50 .
        return false;
    }
    return true;
}

void StatusAreaWindow::sceneChanged(const QList<QRectF> &region)
{
    if (!region.empty() && !statusAreaPixmap->isNull()) {
        QPainter painter(statusAreaPixmap);
        QRectF changeRect(0,0,0,0);
        foreach(const QRectF & r, region) {
            changeRect = changeRect.united(r);
        }
        // Don't draw areas that are outside the pixmap
        if(changeRect.intersects(statusAreaPixmap->rect())) {
            QRectF drawRect = changeRect.intersected(statusAreaPixmap->rect());

            if (painter.isActive()) {
                painter.fillRect(drawRect, QBrush(QColor(0, 0, 0, 0)));
                // Render the changes to the scene
                scene->render(&painter, drawRect, drawRect);
            }
        }
    }
}

StatusAreaWindow::~StatusAreaWindow()
{
    scene->removeItem(statusArea_);
    delete statusArea_;
    delete scene;
}

StatusArea *StatusAreaWindow::statusArea() const
{
    return statusArea_;
}

void StatusAreaWindow::rotate(const Dui::OrientationAngle &angle)
{
    // Set the size of the window
    if (angle == Dui::Angle90 || angle == Dui::Angle270) {
        setFixedSize(28, DuiDeviceProfile::instance()->resolution().height());
    } else {
        setFixedSize(DuiDeviceProfile::instance()->resolution().width(), 28);
    }

    // Move the window to the correct position
    if (angle == Dui::Angle0 || angle == Dui::Angle270) {
        move(0, 0);
    } else if (angle == Dui::Angle90) {
        move(DuiDeviceProfile::instance()->resolution().width() - width(), 0);
    } else {
        move(0, DuiDeviceProfile::instance()->resolution().height() - height());
    }

    // Rotate the view
    QTransform transform;
    transform.rotate(angle);
    setTransform(transform);
}
