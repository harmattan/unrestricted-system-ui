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

#include <MOrientationChangeEvent>
#include <MOnDisplayChangeEvent>
#include <MDeviceProfile>
#include "statusareawindow.h"
#include "statusarea.h"

StatusAreaWindow::StatusAreaWindow(QWidget *parent) :
    MWindow(NULL, parent),
    scene(new QGraphicsScene),
    statusArea_(new StatusArea(NULL,this))
{
    // Set the window type to _NET_WM_WINDOW_TYPE_DOCK
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);

    // Set the scene and add the status area to the scene
    setScene(scene);
    scene->addItem(statusArea_);

    // Rotate to current orientation
    rotate(this->orientationAngle());
    connect(this, SIGNAL(orientationAngleChanged(const M::OrientationAngle &)), this, SLOT(rotate(const M::OrientationAngle &)));
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

void StatusAreaWindow::rotate(const M::OrientationAngle &angle)
{
    // Set the size of the window
    if (angle == M::Angle90 || angle == M::Angle270) {
        setFixedSize(28, MDeviceProfile::instance()->resolution().height());
    } else {
        setFixedSize(MDeviceProfile::instance()->resolution().width(), 28);
    }

    // Move the window to the correct position
    if (angle == M::Angle0 || angle == M::Angle270) {
        move(0, 0);
    } else if (angle == M::Angle90) {
        move(MDeviceProfile::instance()->resolution().width() - width(), 0);
    } else {
        move(0, MDeviceProfile::instance()->resolution().height() - height());
    }

    // Rotate the view
    QTransform transform;
    transform.rotate(angle);
    setTransform(transform);
}
