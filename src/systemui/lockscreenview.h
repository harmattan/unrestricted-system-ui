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

private:
    // The background GConf keys:
    MSceneWindow* controller;
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
