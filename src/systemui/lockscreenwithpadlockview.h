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
#ifndef LOCKSCREENWITHPADLOCKVIEW_H
#define LOCKSCREENWITHPADLOCKVIEW_H

#include "lockscreenview.h"
#include <MOverlay>

class MImageWidget;
class QGraphicsLinearLayout;

class LockScreenWithPadlockView : public LockScreenView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MSceneWindowStyle)

public:
    LockScreenWithPadlockView (MSceneWindow* controller);
    ~LockScreenWithPadlockView ();

    enum dnd_state {
        STATE_NONE = 0,
        STATE_MOVING,
        STATE_MOVING_ACTIVE
    };

signals:
    void unlocked ();

protected:
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);

private slots:
    void redraw ();
    void redrawIdle ();
    void showHideNotifications (bool show);
    void updateDataTime();
    void resetState ();

private:
    void updateDragAndDropIcon ();

    // Needed to show the DnD icon on the window:
    MWindow *dragAndDropWindow;
    // The DnD icon:
    MImageWidget *dragAndDropIcon;
    MOverlay dragAndDropOverlay;
    // The DnD state
    int dragAndDropState;
    // Needed to determine the current DnD states:
    MWidget *lockLiftArea;
    MWidget *lockLandArea;
    // for frame-dropping
    bool drawPending;
    QPointF dragAndDropPosition;
    QGraphicsLinearLayout *layoutPolicy;
    MWidget *notificationArea;
    MSceneWindow* controller;
};

#endif
