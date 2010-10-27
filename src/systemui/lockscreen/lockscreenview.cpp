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
#include "lockscreenview.h"
#include <MGConfItem>
#include <MViewCreator>
#include "lockscreen.h"

const QString GCONF_BG_LANDSCAPE = "/desktop/meego/background/landscape/picture_filename";
const QString GCONF_BG_PORTRAIT = "/desktop/meego/background/portrait/picture_filename";

LockScreenView::LockScreenView(MSceneWindow* controller) : MSceneWindowView(controller),
    layout(new QGraphicsLinearLayout(Qt::Vertical)),
    lockScreenHeader(new MWidgetController),
    controller(controller),
    gconfBgLandscape(new MGConfItem(GCONF_BG_LANDSCAPE, this)),
    gconfBgPortrait(new MGConfItem(GCONF_BG_PORTRAIT, this)),
    landscapePixmap(new QPixmap),
    portraitPixmap(new QPixmap)
{
    // Set the main layout
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    // Create a header for the lock screen
    lockScreenHeader->setViewType("lockScreenHeader");
    layout->addItem(lockScreenHeader);

    connect(gconfBgLandscape, SIGNAL(valueChanged()), this, SLOT(reloadLandscapeBackground()));
    connect(gconfBgPortrait, SIGNAL(valueChanged()), this, SLOT(reloadPortraitBackground()));

    // Load the backgrounds if any...
    reloadLandscapeBackground();
    reloadPortraitBackground();
}

LockScreenView::~LockScreenView()
{
    delete gconfBgLandscape;
    delete gconfBgPortrait;
    delete landscapePixmap;
    delete portraitPixmap;
    delete lockScreenHeader;
}

void LockScreenView::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPixmap *pixmap = (geometry().height() > geometry().width()) ? portraitPixmap : landscapePixmap;

    painter->drawPixmap(0, 0, pixmap->width(), pixmap->height(), *pixmap);
}

void LockScreenView::reloadLandscapeBackground ()
{
    QString filename = gconfBgLandscape->value().toString();
    if (!filename.isEmpty()) {
        landscapePixmap->load(filename);
    }
}

void LockScreenView::reloadPortraitBackground ()
{
    QString filename = gconfBgPortrait->value().toString();
    if (!filename.isEmpty()) {
        portraitPixmap->load (filename);
    }
}

M_REGISTER_VIEW_NEW(LockScreenView, LockScreen)
