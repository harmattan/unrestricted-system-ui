/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#ifndef EVENTEATER_H
#define EVENTEATER_H

#include <QWidget>

class QMouseEvent;
class QShowEvent;

class EventEater : public QWidget
{
    Q_OBJECT

public:
    EventEater ();
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);

protected:
    virtual void showEvent(QShowEvent *event);

signals:
    void OneInput ();
#ifdef UNIT_TEST
    friend class Ut_LockScreenUI;
    friend class Ut_EventEater;
    friend class Ft_LockScreenUI;
    friend class Ut_LockScreenBusinessLogic;
    friend class Ft_LockScreenBusinessLogic;
#endif
};

#endif
