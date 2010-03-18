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
#include "ut_unlockslider.h"

#include <DuiApplication>
#include <DuiTheme>

void Ut_UnlockSlider::init()
{
    m_tmp = NULL;
    m_subject = new UnlockSlider();
}

void Ut_UnlockSlider::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

DuiApplication *app;
void Ut_UnlockSlider::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_unlockslider";
    app = new DuiApplication(argc, &app_name);
}

void Ut_UnlockSlider::cleanupTestCase()
{
    delete app;
}

void Ut_UnlockSlider::testConstructionAndDestruction()
{
    m_tmp = new UnlockSlider();
    QVERIFY(m_tmp->model() != NULL);
    delete m_tmp;
    m_tmp = NULL;
}

void Ut_UnlockSlider::testHandlePressed()
{
    QVERIFY(m_subject->isHandlePressed() == false);
}

void Ut_UnlockSlider::testPosition()
{
    m_subject->setMagnetic(false);
    QVERIFY(m_subject->isMagnetic() == false);
    for (qreal i = 0.0f; i < 1.0f; i += 0.1f) {
        m_subject->setPosition(i);
        QVERIFY(m_subject->position() == i);
    }
    m_subject->setPosition(-1.0f);
    QVERIFY(m_subject->position() == 0.0f);
    m_subject->setPosition(2.0f);
    QVERIFY(m_subject->position() == 1.0f);
    m_subject->setMagnetic(true);
}

void Ut_UnlockSlider::testIconVisible()
{
    m_subject->setIconVisible(false);
    QVERIFY(m_subject->isIconVisible() == false);
    m_subject->setIconVisible(true);
    QVERIFY(m_subject->isIconVisible() == true);
}

void Ut_UnlockSlider::testMagnetic()
{
    m_subject->setMagnetic(false);
    QVERIFY(m_subject->isMagnetic() == false);
    m_subject->setMagnetic(true);
    QVERIFY(m_subject->isMagnetic() == true);
}

void Ut_UnlockSlider::testOrientation()
{
    m_subject->setOrientation(Qt::Horizontal);
    QVERIFY(m_subject->orientation() == Qt::Horizontal);
    m_subject->setOrientation(Qt::Vertical);
    QVERIFY(m_subject->orientation() == Qt::Vertical);
}

void Ut_UnlockSlider::testReset()
{
    m_subject->reset();
    QVERIFY(m_subject->position() == 0);
}

QTEST_APPLESS_MAIN(Ut_UnlockSlider)
