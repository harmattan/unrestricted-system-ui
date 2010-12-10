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

#include "ut_notificationstatusindicator.h"
#include <MApplication>

void Ut_NotificationStatusIndicator::init()
{
    m_subject = new NotificationStatusIndicator;
}

void Ut_NotificationStatusIndicator::cleanup()
{
    delete m_subject;
}

void Ut_NotificationStatusIndicator::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_notificationstatusindicator";
    app = new MApplication(argc, &app_name);
}

void Ut_NotificationStatusIndicator::cleanupTestCase()
{
    delete app;
}

void Ut_NotificationStatusIndicator::testSetActive()
{
    connect(this, SIGNAL(notifierSinkActive(bool)), m_subject, SLOT(setActive(bool)));

    emit notifierSinkActive(true);
    QVERIFY(m_subject->objectName().indexOf("On") >= 0);

    emit notifierSinkActive(false);
    QVERIFY(m_subject->objectName().indexOf("On") < 0);
}

QTEST_APPLESS_MAIN(Ut_NotificationStatusIndicator)
