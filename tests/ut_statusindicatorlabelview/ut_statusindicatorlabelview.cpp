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

#include <QtTest/QtTest>
#include "ut_statusindicatorlabelview.h"
#include "statusindicatorlabelview.h"
#include "inputmethodstatusindicatoradaptor_stub.h"

#include <MApplication>
#include <MLabel>

TestStatusIndicatorLabelView::TestStatusIndicatorLabelView(StatusIndicator *controller) :
    StatusIndicatorLabelView(controller)
{
}

StatusIndicatorModel *TestStatusIndicatorLabelView::getModel()
{
    return model();
}

void Ut_StatusIndicatorLabelView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatorlabelview";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicatorLabelView::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorLabelView::init()
{
    controller = new StatusIndicator;
    m_subject = new TestStatusIndicatorLabelView(controller);
    controller->setView(m_subject);
}

void Ut_StatusIndicatorLabelView::cleanup()
{
    delete m_subject;
}

void Ut_StatusIndicatorLabelView::testLabelChanged()
{
    m_subject->getModel()->setValue("newValue");
    QCOMPARE(m_subject->label->text(), QString("newValue"));
}

void Ut_StatusIndicatorLabelView::testStylePostfixChanged()
{
    controller->setObjectName("Test");
    m_subject->getModel()->setStylePostfix("Postfix1");
    QCOMPARE(m_subject->label->styleName(), QString("StatusIndicatorLabelPostfix1"));

    controller->setObjectName("TestPortrait");
    m_subject->getModel()->setStylePostfix("Postfix2");
    QCOMPARE(m_subject->label->styleName(), QString("StatusIndicatorLabelPortraitPostfix2"));
}

void Ut_StatusIndicatorLabelView::testModelChanged()
{
    controller->setObjectName("TestLandscape");
    StatusIndicatorModel *m = new StatusIndicatorModel;
    m->setValue("newValue");
    m_subject->setModel(m);
    QCOMPARE(m_subject->label->text(), QString("newValue"));
    QCOMPARE(m_subject->label->styleName(), QString("StatusIndicatorLabelLandscape"));

    controller->setObjectName("TestPortrait");
    m = new StatusIndicatorModel;
    m->setValue("anotherValue");
    m->setStylePostfix("TestPostfix");
    m_subject->setModel(m);
    QCOMPARE(m_subject->label->text(), QString("anotherValue"));
    QCOMPARE(m_subject->label->styleName(), QString("StatusIndicatorLabelPortraitTestPostfix"));
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorLabelView)
