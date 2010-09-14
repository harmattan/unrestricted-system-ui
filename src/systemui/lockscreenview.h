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
#ifndef LOCKSCREENVIEW_H
#define LOCKSCREENVIEW_H

#include <mscenewindowview.h>

class MGConfItem;
class QPixmap;

class LockScreenView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MSceneWindowStyle)

public:
    LockScreenView(MSceneWindow* controller);
    ~LockScreenView();
signals:
    void unlocked ();

protected:
    //! \reimp
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    //! \reimp_end

private slots:
    void reloadLandscapeBackground ();
    void reloadPortraitBackground ();
    void redraw ();
    void redrawIdle ();

private:
    // The background GConf keys:
    MGConfItem *gconfBgLandscape;
    MGConfItem *gconfBgPortrait;
    MSceneWindow* controller;

    Q_DISABLE_COPY(LockScreenView)
};

#endif // LOCKSCREENVIEW_H
