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
#include "ut_unlockwidgets.h"
#include <debug.h>
#include <MTheme>
#include <MApplication>
#include <unlockwidgets.h>
#include <MImageWidget>

int   argc = 1;
char *argv[] = {
    (char *) "./ut_unlockwidgets",
    NULL };
const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

MApplication    *m_App;

void ut_unlockwidgets::initTestCase ()
{
    m_App = new MApplication(argc, argv);

    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
}

void ut_unlockwidgets::cleanupTestCase ()
{
    m_App->deleteLater();
}

void ut_unlockwidgets::init ()
{

}

void ut_unlockwidgets::cleanup ()
{

}

void ut_unlockwidgets::test_unlock_header ()
{
    UnlockHeader *header = new UnlockHeader;

    header->setActive (true);

    MImageWidget *icon = header->m_icon;

    QVERIFY (icon != 0);
    if (icon != 0)
        QVERIFY (icon->isVisible () == true);

    header->setActive (false);

    if (icon != 0)
        QVERIFY (icon->isVisible () == false);

    delete header;
}

void ut_unlockwidgets::test_unlock_area ()
{
    UnlockArea  *area = new UnlockArea;

    MImageWidget *icon = area->m_unlock_icon;

    QVERIFY (icon != 0);

    area->setEnabled (false);

    QVERIFY (area->objectName () == "LockLandArea");

    area->setEnabled (true);

    QVERIFY (area->objectName () == "LockLandAreaDragged");

    area->setActive (true);

    QVERIFY (area->objectName () == "LockLandAreaActive");

    delete area;
}

void ut_unlockwidgets::test_unlock_missedevents ()
{
    UnlockNotifications *missedEvents =
        new UnlockNotifications;

    // TODO: how this can be tested properly?

    delete missedEvents;
}

QTEST_APPLESS_MAIN (ut_unlockwidgets)

