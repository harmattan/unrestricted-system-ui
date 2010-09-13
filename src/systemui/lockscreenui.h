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
#ifndef LOCKSCREENUI_H
#define LOCKSCREENUI_H

#include <MWindow>

class MWidget;
class QGraphicsLinearLayout;
class QShowEvent;
class LockScreenWindow;

class LockScreenUI : public MWindow
{
    Q_OBJECT

public:
    LockScreenUI ();
    virtual ~LockScreenUI();

signals:
    void unlocked ();

public slots:
    void updateDateTime ();
    void reset ();

protected:
    void createContent ();
    virtual void showEvent(QShowEvent *event);

private slots:
    void realize ();
    void sliderUnlocked ();
    void showHideNotifications (bool show);

private:
    bool                   m_Realized;
    QGraphicsLinearLayout *m_policy;
    LockScreenWindow      *m_SceneWindow;

    MWidget         *m_notificationArea;
    MWidget         *m_LockLiftArea;
    MWidget         *m_LockLandArea;

#ifdef UNIT_TEST
    friend class Ut_LockScreenUI;
    friend class Ut_LockScreenWindow;
    friend class Ft_LockScreenUI;
    friend class Ut_LockScreenBusinessLogic;
    friend class Ft_LockScreenBusinessLogic;
#endif
};

#endif
