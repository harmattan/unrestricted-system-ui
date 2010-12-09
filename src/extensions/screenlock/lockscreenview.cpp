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
#include <QGraphicsLinearLayout>
#include <MGConfItem>
#include <MViewCreator>
#include <MSceneManager>
#include "lockscreen.h"
#include "lockscreenview.h"
#include "lockscreenheader.h"

const QString GCONF_KEY_LANDSCAPE = "/desktop/meego/background/landscape/picture_filename";
const QString GCONF_KEY_PORTRAIT = "/desktop/meego/background/portrait/picture_filename";

LockScreenBackgroundPixmap::LockScreenBackgroundPixmap(const QString &gConfKey) :
    gConfItem(new MGConfItem(gConfKey, this)),
    pixmap(NULL),
    pixmapFromTheme(false)
{
    updatePixmap();

    connect(gConfItem, SIGNAL(valueChanged()), this, SLOT(updatePixmap()));
}

LockScreenBackgroundPixmap::~LockScreenBackgroundPixmap()
{
    destroyPixmap();
}

void LockScreenBackgroundPixmap::updatePixmap()
{
    // Destroy the old pixmap
    destroyPixmap();

    QString image = gConfItem->value().toString();
    if (!image.isEmpty()) {
        if (image.startsWith('/')) {
            // Load from absolute path: if it fails the pixmap will be invalid and the default will be used
            pixmapFromTheme = false;
            pixmap = new QPixmap;
            pixmap->load(image);
        } else {
            // Load from theme
            pixmapFromTheme = true;
            pixmap = const_cast<QPixmap *>(MTheme::pixmap(image));
        }
    }

    // Let the view know about the updated pixmap
    emit pixmapUpdated();
}

void LockScreenBackgroundPixmap::destroyPixmap()
{
    if (pixmap != NULL) {
        if (pixmapFromTheme) {
            MTheme::releasePixmap(pixmap);
        } else {
            delete pixmap;
        }
        pixmap = NULL;
    }
}

LockScreenView::LockScreenView(MWidgetController* controller) : MWidgetView(controller),
    controller(controller),
    layout(new QGraphicsLinearLayout(Qt::Vertical)),
    lockScreenHeader(new LockScreenHeader),
    landscapePixmap(GCONF_KEY_LANDSCAPE),
    portraitPixmap(GCONF_KEY_PORTRAIT)
{
    // Set the main layout
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    // Add a header for the lock screen
    layout->addItem(lockScreenHeader);

    // Update the style name based on whether there are any custom background pixmaps or not
    updateStyleName();
    connect(&landscapePixmap, SIGNAL(pixmapUpdated()), this, SLOT(updateStyleName()));
    connect(&portraitPixmap, SIGNAL(pixmapUpdated()), this, SLOT(updateStyleName()));
}

LockScreenView::~LockScreenView()
{
    delete lockScreenHeader;
}

void LockScreenView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    QPixmap *pixmap = controller->sceneManager()->orientation() == M::Landscape ? landscapePixmap.pixmap : portraitPixmap.pixmap;

    if (pixmap == NULL || pixmap->isNull()) {
        // If no specific pixmap has been loaded or the pixmap is not valid, use the default
        MWidgetView::drawBackground(painter, option);
    } else {
        // Otherwise draw the specific pixmap
        painter->drawPixmap(QRectF(QPointF(), controller->geometry().size()), *pixmap, QRectF(0, 0, pixmap->width(), pixmap->height()));
    }
}

void LockScreenView::updateStyleName()
{
    // Set the style name to LockScreenWithCustomBackground only if there is a valid custom pixmap for both orientations
    controller->setStyleName((landscapePixmap.pixmap != NULL && !landscapePixmap.pixmap->isNull() &&
                              portraitPixmap.pixmap != NULL && !portraitPixmap.pixmap->isNull()) ?
                              "LockScreenWithCustomBackground" : "LockScreenWithDefaultBackground");

    // Refresh the display
    update();
}

M_REGISTER_VIEW_NEW(LockScreenView, LockScreen)
