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
#ifndef LOCKSCREENWINDOW_H
#define LOCKSCREENWINDOW_H

#include <MWindow>

class LockScreen;

class LockScreenWindow : public MWindow
{
    Q_OBJECT

public:
    LockScreenWindow(QWidget *parent = NULL);
    virtual ~LockScreenWindow();
    void reset();

protected:
    /*!
     * Sets the _MEEGO_STACKING_LAYER window property to 2.
     */
    virtual void showEvent(QShowEvent *event);

signals:
    void unlocked();

private:
    LockScreen *lockScreen;

#ifdef UNIT_TEST
    friend class Ut_LockScreenWindow;
#endif
};

#endif
