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
#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <MWidgetController>

class LockScreen : public MWidgetController
{
    Q_OBJECT

public:
    LockScreen(QGraphicsItem *parent = NULL);
    virtual ~LockScreen();
    void reset();

signals:
    void unlocked();
    void resetRequested();

private slots:
    //! Unlocks the screen lock
    void unlock();

#ifdef UNIT_TEST
    friend class Ut_LockScreen;
#endif
};

#endif
