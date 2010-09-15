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
****************************************************************************/
#include "lockscreenview.h"
#include <MGConfItem>
#include <MViewCreator>
#include "lockscreen.h"

const QString GCONF_BG_LANDSCAPE = "/desktop/meego/background/landscape/picture_filename";
const QString GCONF_BG_PORTRAIT = "/desktop/meego/background/portrait/picture_filename";

LockScreenView::LockScreenView(MSceneWindow* controller) : MSceneWindowView(controller),
    controller(controller),
    gconfBgLandscape(new MGConfItem (GCONF_BG_LANDSCAPE, this)),
    gconfBgPortrait(new MGConfItem (GCONF_BG_PORTRAIT, this)),
    landscapePixmap(new QPixmap),
    portraitPixmap(new QPixmap)
{
    connect (gconfBgLandscape, SIGNAL(valueChanged()), this, SLOT(reloadLandscapeBackground()));
    connect (gconfBgPortrait, SIGNAL(valueChanged()), this, SLOT(reloadPortraitBackground()));

    // Load the backgrounds if any...
    reloadLandscapeBackground ();
    reloadPortraitBackground ();

    setObjectName ("LockScreenWindow");
}

LockScreenView::~LockScreenView()
{
    delete gconfBgLandscape;
    delete gconfBgPortrait;
    delete landscapePixmap;
    delete portraitPixmap;
}

void LockScreenView::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    bool portrait = geometry().height() > geometry().width ();

    if (portrait) {
        painter->drawPixmap (0, 0, portraitPixmap->width(), portraitPixmap->height(), *portraitPixmap);
    } else {
        painter->drawPixmap (0, 0, landscapePixmap->width(), landscapePixmap->height(), *landscapePixmap);
    }
}

void LockScreenView::reloadLandscapeBackground ()
{
    QVariant value = gconfBgLandscape->value();
    QString filename = value.toString();
    if (!filename.isEmpty()) {
        landscapePixmap->load (filename);
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
