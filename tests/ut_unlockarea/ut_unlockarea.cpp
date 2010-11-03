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
#include "ut_unlockarea.h"
#include <MTheme>
#include <MApplication>
#include <unlockarea.h>
#include <MImageWidget>

int   argc = 1;
char *argv[] = {
    (char *) "./ut_unlockarea",
    NULL };

MApplication    *m_App;

void ut_unlockarea::initTestCase ()
{
    m_App = new MApplication(argc, argv);
}

void ut_unlockarea::cleanupTestCase ()
{
    m_App->deleteLater();
}

void ut_unlockarea::init ()
{

}

void ut_unlockarea::cleanup ()
{

}

void ut_unlockarea::test_unlock_area ()
{
    UnlockArea  *area = new UnlockArea;

    MWidget *icon = area->m_unlock_icon;

    QVERIFY (icon != 0);

    area->setEnabled (false);

    QVERIFY (area->objectName () == "LockLandArea");

    area->setEnabled (true);

    QVERIFY (area->objectName () == "LockLandAreaDragged");

    area->setActive (true);

    QVERIFY (area->objectName () == "LockLandAreaActive");

    delete area;
}

QTEST_APPLESS_MAIN (ut_unlockarea)

