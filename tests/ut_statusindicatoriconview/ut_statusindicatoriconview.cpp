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

#include <MApplication>
#include "ut_statusindicatoriconview.h"
#include "statusindicatoriconview.h"
#include "inputmethodstatusindicatoradaptor_stub.h"
#include "statusindicatoranimationview_stub.h"

TestStatusIndicatorIconView::TestStatusIndicatorIconView(StatusIndicator *controller) :
    StatusIndicatorIconView(controller)
{
}

StatusIndicatorModel *TestStatusIndicatorIconView::getModel()
{
    return model();
}

void TestStatusIndicatorIconView::executeStyleChanged()
{
    applyStyle();
}

void TestStatusIndicatorIconView::setImageList(const QStringList &imageList)
{
    this->imageList = imageList;
    images = QVector<const QPixmap *>(imageList.length(), NULL);
}

StatusIndicatorIconStyle *TestStatusIndicatorIconView::modifiableStyle()
{
    StatusIndicatorIconStyleContainer &sc = style();
    const StatusIndicatorIconStyle *const_s = sc.operator ->();
    StatusIndicatorIconStyle *s = const_cast<StatusIndicatorIconStyle *>(const_s);
    return s;
}

void Ut_StatusIndicatorIconView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatoranimationview";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicatorIconView::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorIconView::init()
{
    // Construct the test subject
    controller = new StatusIndicator;
    m_subject = new TestStatusIndicatorIconView(controller);
    m_subject->setModel(controller->model());
    gStatusIndicatorAnimationViewStub->stubReset();
    gStatusIndicatorAnimationViewStub->stubSetReturnValue("model", controller->model());

    connect(this, SIGNAL(updateData(const QList<const char *>&)), m_subject, SLOT(updateData(const QList<const char *>&)));
}

void Ut_StatusIndicatorIconView::cleanup()
{
    delete m_subject;
    delete controller;
}

void Ut_StatusIndicatorIconView::testUpdateData()
{
    m_subject->setImageList(QStringList() << "1" << "2" << "3" << "4" << "5" << "6");
    m_subject->getModel()->setValue(0.5);
    QList<const char *> modifications;
    modifications << StatusIndicatorModel::Value;
    emit updateData(modifications);
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubCallCount("setAnimationFrame"), 1);
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubLastCallTo("setAnimationFrame").parameter<int>(0), 3);
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubCallCount("setFirstAnimationFrame"), 1);
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubLastCallTo("setFirstAnimationFrame").parameter<int>(0), 3);
}

void Ut_StatusIndicatorIconView::testApplyStyle()
{
    // An image list of 6 items and a model value of 0.5 should result in an animation frame = 3
    m_subject->getModel()->setValue(0.5);
    // A call to setImageList() simulates the StatusIndicatorAnimationView::setupImageList() that will be
    // called during the applyStyle() call
    m_subject->setImageList(QStringList() << "1" << "2" << "3" << "4" << "5" << "6");

    // Verify that setupImageList() is called duringthe applyStyle() call
    QString imageList("1 2 3 4 5 6");
    m_subject->modifiableStyle()->setImageList(imageList);
    m_subject->executeStyleChanged();
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubCallCount("setupImageList"), 1);
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubLastCallTo("setupImageList").parameter<QString>(0), imageList);

    // Verify that the correct animation frame is set during the applyStyle() call
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubCallCount("setAnimationFrame"), 1);
    QCOMPARE(gStatusIndicatorAnimationViewStub->stubLastCallTo("setAnimationFrame").parameter<int>(0), 3);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorIconView)
