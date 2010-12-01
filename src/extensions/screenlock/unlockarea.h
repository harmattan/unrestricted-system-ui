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
#ifndef _UNLOCKAREA_H
#define _UNLOCKAREA_H

class MLabel;
class MWidget;
class MImageWidget;

#include <QObject>
#include <MLocale>
#include <MStylableWidget>

#ifdef HAVE_QMSYSTEM
#include <qmtime.h>
#endif

#include "unlocknotifications.h"

class UnlockArea : public MStylableWidget
{
    Q_OBJECT

public:
    UnlockArea(QGraphicsItem *parent = NULL);
    virtual ~UnlockArea();

    void setEnabled(bool enabled);
    virtual void setActive(bool active);

private:
    void updateState();

    MWidget         *m_unlock_icon;
    bool             m_enabled;
    bool             m_active;

#ifdef UNIT_TEST
    friend class ut_unlockarea;
#endif
};

#endif
