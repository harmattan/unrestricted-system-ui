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
#ifndef UT_UNLOCKSLIDERWIDGET_H
#define UT_UNLOCKSLIDERWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

#include "unlockslider.h"

//Q_DECLARE_METATYPE(MButton*);

class Ut_UnlockSlider : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstructionAndDestruction();
    void testHandlePressed();
    void testPosition();
    void testIconVisible();
    void testMagnetic();
    void testOrientation();
    void testReset();

    // signals:
    // -unlocked
    // -released

    // blink

private:
    UnlockSlider* m_subject;
    UnlockSlider* m_tmp;
};

#endif
