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
#include <MStyle>
#include "statusareastyle.h"

#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDebug>

StatusAreaWindow::StatusAreaWindow(QWidget *parent) :
    MWindow(NULL, parent),
    scene(new QGraphicsScene),
    statusArea_(new StatusArea(NULL,this)),
    statusAreaPixmap(NULL),
    displayState(new Maemo::QmDisplayState()),
    renderScene(true)
{
    scene->addItem(statusArea_);
    // Get signaled when the scene changes
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(sceneChanged(QList<QRectF>)));
    connect(displayState, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)), this, SLOT(setSceneRender(Maemo::QmDisplayState::DisplayState)));
    setSizeFromStyle();
    if(!createSharedPixmapHandle()) {
        qWarning() << "Shared Pixmap was not created. Status area will not render";
    }
}

void StatusAreaWindow::setSizeFromStyle()
{
    const StatusAreaStyle *style = static_cast<const StatusAreaStyle *> (MTheme::style("StatusAreaStyle", "", "", "", M::Landscape, NULL));
    if(style) {
        statusAreaHeight = style->preferredSize().height();
        statusAreaWidth = style->preferredSize().width();
    }
}

bool StatusAreaWindow::createSharedPixmapHandle()
{
    // Create a pixmap in which top portion TopLeft(0,0) BottomRight(status area width,status area height) is landscpae.
    // Bottom portion TopLeft(0,status area height) BottomRight(status area portrait width,2*status area height) is portrait. unused portion is portrait is not drawn when in portrait
    statusAreaPixmap = new QPixmap(statusAreaWidth, 2*statusAreaHeight);
    QApplication::syncX();
    QFile handleTempFile(QDir::temp().filePath("mstatusbar_pixmap_handle"));
    if (!handleTempFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QDataStream dataStream(&handleTempFile);
    dataStream << static_cast<quint32> (statusAreaPixmap->handle());
    return true;
}

StatusAreaWindow::~StatusAreaWindow()
{
    scene->removeItem(statusArea_);
    delete statusArea_;
    delete scene;
    delete statusAreaPixmap;
    delete displayState;
}

void StatusAreaWindow::sceneChanged(const QList<QRectF> &region)
{
    if (!region.empty() && !statusAreaPixmap->isNull() && renderScene) {
        QPainter painter(statusAreaPixmap);
        QRectF changeRect(0,0,0,0);
        foreach(const QRectF & r, region) {
            changeRect = changeRect.united(r);
        }

	// Don't draw areas that are outside the pixmap
        if(changeRect.intersects(statusAreaPixmap->rect())) {
            QRectF sourceRect = changeRect.intersected(statusAreaPixmap->rect());
            if (painter.isActive()) {
                painter.fillRect(sourceRect, QColor(Qt::black));
                scene->render(&painter, sourceRect, sourceRect);
            }
        }
    }
}

void StatusAreaWindow::setSceneRender(Maemo::QmDisplayState::DisplayState state)
{
    switch(state) {
    case Maemo::QmDisplayState::Dimmed:
    case Maemo::QmDisplayState::Off:
        renderScene = false;
        break;
    case Maemo::QmDisplayState::On:
        renderScene = true;
        break;
    }
}

