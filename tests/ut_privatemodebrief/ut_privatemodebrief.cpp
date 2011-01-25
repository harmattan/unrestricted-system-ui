/***************************************************************************
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

#include <QtTest/QtTest>
#include <DcpWidgetTypes>
#include "ut_privatemodebrief.h"
#include "privatemodebrief.h"

QString g_gconf_key;
QVariant g_gconf_value;

MGConfItem::MGConfItem(const QString &key, QObject *parent)
{
    Q_UNUSED(parent);
    g_gconf_key = key;
}

MGConfItem::~MGConfItem()
{
}

void MGConfItem::set(const QVariant &val)
{
    g_gconf_value = val;
}

QVariant MGConfItem::value() const
{
    return g_gconf_value;
}

void Ut_PrivateModeBrief::initTestCase()
{
}

void Ut_PrivateModeBrief::cleanupTestCase()
{
}

void Ut_PrivateModeBrief::init()
{
    m_subject = new PrivateModeBrief();
    QVERIFY(m_subject->widgetTypeID() == DcpWidgetType::Toggle);
}

void Ut_PrivateModeBrief::cleanup()
{
    delete m_subject;
}

void Ut_PrivateModeBrief::testGConfKey()
{
    QVERIFY(g_gconf_key == "/desktop/meego/privacy/private_lockscreen_notifications");

    QSignalSpy changeSpy(m_subject, SIGNAL(valuesChanged()));

    m_subject->setToggle(true);
    QCOMPARE(changeSpy.count(), 1);
    QCOMPARE(g_gconf_value.toBool(), (bool)true);
    QCOMPARE(m_subject->toggle(), (bool)true);

    m_subject->setToggle(false);
    QCOMPARE(changeSpy.count(), 2);
    QCOMPARE(g_gconf_value.toBool(), (bool)false);
    QCOMPARE(m_subject->toggle(), (bool)false);
}

QTEST_APPLESS_MAIN(Ut_PrivateModeBrief)
