/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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
#include <MApplication>
#include "ut_template.h"

void Ut_Template::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_template" };
    app = new MApplication(argc, app_name);
}

void Ut_Template::cleanupTestCase()
{
    delete app;
}

void Ut_Template::init()
{
//    m_subject = new Template();
}

void Ut_Template::cleanup()
{
//    delete m_subject;
}

QTEST_APPLESS_MAIN(Ut_Template)
