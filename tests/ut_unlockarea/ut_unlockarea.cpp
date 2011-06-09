/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system-ui.
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
#include <QtTest/QtTest>
#include "ut_unlockarea.h"
#include <MApplication>
#include <unlockarea.h>

int   argc = 1;
char *argv[] = {
   (char *) "./ut_unlockarea",
    NULL };

MApplication    *m_App;

void Ut_UnlockArea::initTestCase()
{
    m_App = new MApplication(argc, argv);
}

void Ut_UnlockArea::cleanupTestCase()
{
    m_App->deleteLater();
}

void Ut_UnlockArea::init()
{

}

void Ut_UnlockArea::cleanup()
{

}

void Ut_UnlockArea::testUnlockArea()
{
    UnlockArea *area = new UnlockArea;

    MWidget *icon = area->m_unlock_icon;

    QVERIFY(icon != 0);

    area->setEnabled(false);

    QCOMPARE(area->objectName(), QString("LockLandArea"));

    area->setEnabled(true);

    QCOMPARE(area->objectName(), QString("LockLandAreaDragged"));

    area->setActive(true);

    QCOMPARE(area->objectName(), QString("LockLandAreaActive"));

    delete area;
}

QTEST_APPLESS_MAIN(Ut_UnlockArea)

