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
#ifndef LOCKSCREENVIEW_H
#define LOCKSCREENVIEW_H

#include <mscenewindowview.h>

class QGraphicsLinearLayout;
class MGConfItem;
class QPixmap;
class MWidgetController;

class LockScreenView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MSceneWindowStyle)

public:
    LockScreenView(MSceneWindow* controller);
    ~LockScreenView();

signals:
    void unlocked ();

private slots:
    void reloadLandscapeBackground ();
    void reloadPortraitBackground ();

protected:
    //! \reimp
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    //! \reimp_end

    //! Layout for the view
    QGraphicsLinearLayout *layout;
    //! lock screen header widget
    MWidgetController *lockScreenHeader;

private:
    MSceneWindow* controller;

    // The background GConf keys:
    MGConfItem *gconfBgLandscape;
    MGConfItem *gconfBgPortrait;
    QPixmap* landscapePixmap;
    QPixmap* portraitPixmap;

    Q_DISABLE_COPY(LockScreenView)

#ifdef UNIT_TEST
    friend class Ut_LockScreenView;
#endif
};

#endif // LOCKSCREENVIEW_H
