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
#include "statusarearenderer.h"
#include "statusarea.h"
#include <MStyle>
#include "statusareastyle.h"
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QX11Info>
#include "x11wrapper.h"

StatusAreaRenderer::StatusAreaRenderer(QObject *parent) :
    QObject(parent),
    scene(new QGraphicsScene),
    statusArea(new StatusArea),
    statusAreaPixmap(NULL),
#ifdef HAVE_QMSYSTEM
    displayState(new Maemo::QmDisplayState()),
#endif
    renderScene(true)
{
    scene->setParent(this);
    scene->setObjectName("statusareascene");
    scene->addItem(statusArea);
    statusArea->setObjectName("statusarea");
    connect(this, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), statusArea, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)));

    // Get signaled when the scene changes
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(sceneChanged(QList<QRectF>)));
#ifdef HAVE_QMSYSTEM
    connect(displayState, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)), this, SLOT(setSceneRender(Maemo::QmDisplayState::DisplayState)));
#endif

    setSizeFromStyle();
    if(!createSharedPixmapHandle()) {
        qWarning() << "Shared Pixmap was not created. Status area will not render";
    }
}

void StatusAreaRenderer::setSizeFromStyle()
{
    const StatusAreaStyle *style = static_cast<const StatusAreaStyle *> (MTheme::style("StatusAreaStyle", "", "", "", M::Landscape, NULL));
    if(style) {
        statusAreaHeight = style->preferredSize().height();
        statusAreaWidth = style->preferredSize().width();
    }
}

bool StatusAreaRenderer::createSharedPixmapHandle()
{
    // Create a pixmap in which top portion TopLeft(0,0) BottomRight(status area width,status area height) is landscpae.
    // Bottom portion TopLeft(0,status area height) BottomRight(status area portrait width,2*status area height) is portrait. unused portion is portrait is not drawn when in portrait

    Pixmap pixmap = X11Wrapper::XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(),
                                              statusAreaWidth, 2*statusAreaHeight, 16);
    QApplication::syncX();
    statusAreaPixmap = new QPixmap();
    *statusAreaPixmap = QPixmap::fromX11Pixmap(pixmap, QPixmap::ExplicitlyShared);

    if (!statusAreaPixmap->isNull()) {
        return true;
    } else {
        return false;
    }
}

uint StatusAreaRenderer::sharedPixmapHandle()
{
    return static_cast<quint32> (statusAreaPixmap->handle());
}

StatusAreaRenderer::~StatusAreaRenderer()
{
    scene->removeItem(statusArea);
    delete statusArea;
    delete statusAreaPixmap;

#ifdef HAVE_QMSYSTEM
    delete displayState;
#endif
}

void StatusAreaRenderer::sceneChanged(const QList<QRectF> &region)
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
                QApplication::syncX();
            }
        }
    }
}

#ifdef HAVE_QMSYSTEM
void StatusAreaRenderer::setSceneRender(Maemo::QmDisplayState::DisplayState state)
{
    bool oldRenderScene = renderScene;

    switch(state) {
    case Maemo::QmDisplayState::Dimmed:
    case Maemo::QmDisplayState::Off:
        renderScene = false;
        break;
    case Maemo::QmDisplayState::On:
        renderScene = true;
        break;
    default:
        renderScene = true;
    }

    if (renderScene != oldRenderScene) {
        MOnDisplayChangeEvent::State state = renderScene ?
           MOnDisplayChangeEvent::FullyOnDisplay : MOnDisplayChangeEvent::FullyOffDisplay;

        MOnDisplayChangeEvent event(state, QRectF());
        foreach(QGraphicsItem *item, scene->items()) {
            scene->sendEvent(item, &event);
        }
    }
}
#endif
