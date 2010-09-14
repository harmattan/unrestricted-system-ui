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

const QString GCONF_BG_LANDSCAPE = "/desktop/meego/background/landscape/picture_filename";
const QString GCONF_BG_PORTRAIT = "/desktop/meego/background/portrait/picture_filename";

LockScreenView::LockScreenView(MSceneWindow* controller) : MSceneWindowView(controller),
    controller(controller),
    gconfBgLandscape(new MGConfItem (GCONF_BG_LANDSCAPE, this)),
    gconfBgPortrait(new MGConfItem (GCONF_BG_PORTRAIT, this))
{
}

LockScreenView::~LockScreenView()
{
}

void LockScreenView::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void LockScreenView::reloadLandscapeBackground ()
{

}

void LockScreenView::reloadPortraitBackground ()
{

}

void LockScreenView::redraw ()
{

}

void LockScreenView::redrawIdle ()
{

}

