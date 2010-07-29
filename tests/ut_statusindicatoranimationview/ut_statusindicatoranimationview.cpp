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
#include <MApplication>
#include "ut_statusindicatoranimationview.h"
#include "statusindicatoriconview.h"
#include "statusindicator.h"
#include "inputmethodstatusindicatoradaptor_stub.h"

TestStatusIndicatorAnimationView::TestStatusIndicatorAnimationView(StatusIndicator *controller) :
    StatusIndicatorAnimationView(controller)
{
}

StatusIndicatorModel *TestStatusIndicatorAnimationView::getModel()
{
    return model();
}

void TestStatusIndicatorAnimationView::executeStyleChanged()
{
    applyStyle();
}

void TestStatusIndicatorAnimationView::callableDrawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    drawContents(painter, option);
}

StatusIndicatorAnimationStyle *TestStatusIndicatorAnimationView::modifiableStyle()
{
    StatusIndicatorAnimationStyleContainer &sc = style();
    const StatusIndicatorAnimationStyle *const_s = sc.operator ->();
    StatusIndicatorAnimationStyle *s = const_cast<StatusIndicatorAnimationStyle *>(const_s);
    return s;
}

// MTheme stubs
QHash<QPixmap *, QString> mThemePixmapPixmaps;
QPixmap *MTheme::pixmapCopy(const QString &id, const QSize &)
{
    QPixmap *p = new QPixmap;
    mThemePixmapPixmaps[p] = id;

    return p;
}

// QPainter stubs
QList<QPointF> qPainterDrawPixmapPoints;
QList<const QPixmap *> qPainterDrawPixmapPixmaps;
void QPainter::drawPixmap(const QPointF &point, const QPixmap &pixmap)
{
    qPainterDrawPixmapPoints.append(point);
    qPainterDrawPixmapPixmaps.append(&pixmap);
}

// QTimeLine stubs
int qTimeLineDuration;
void QTimeLine::setDuration(int duration)
{
    qTimeLineDuration = duration;
}
QPair<int, int> qTimeLineFrameRange(-1, -1);
void QTimeLine::setFrameRange(int startFrame, int endFrame)
{
    qTimeLineFrameRange = qMakePair(startFrame, endFrame);
}
int qTimeLineUpdateInterval = -1;
void QTimeLine::setUpdateInterval(int interval)
{
    qTimeLineUpdateInterval = interval;
}

QTimeLine::State qTimeLineState = QTimeLine::NotRunning;
bool qTimeLineStartedCalled = false;
bool qTimeLineStoppedCalled = false;
void QTimeLine::start()
{
    qTimeLineStartedCalled = true;
    qTimeLineState = QTimeLine::Running;
}
void QTimeLine::stop()
{
    qTimeLineStoppedCalled = true;
    qTimeLineState = QTimeLine::NotRunning;
}
QTimeLine::State QTimeLine::state() const
{
    return qTimeLineState;
}

void Ut_StatusIndicatorAnimationView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_statusindicatoriconview" };
    app = new MApplication(argc, app_name);
}

void Ut_StatusIndicatorAnimationView::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorAnimationView::init()
{
    mThemePixmapPixmaps.clear();
    qPainterDrawPixmapPoints.clear();
    qPainterDrawPixmapPixmaps.clear();
    qTimeLineDuration = -1;
    qTimeLineFrameRange = qMakePair(-1, -1);
    qTimeLineUpdateInterval = -1;
    qTimeLineState = QTimeLine::NotRunning;
    qTimeLineStartedCalled = false;
    qTimeLineStoppedCalled = false;
    controller = new StatusIndicator;
    m_subject = new TestStatusIndicatorAnimationView(controller);
    m_subject->setModel(controller->model());
}

void Ut_StatusIndicatorAnimationView::cleanup()
{
    delete m_subject;
    delete controller;
}

void Ut_StatusIndicatorAnimationView::testSetupModel()
{
    m_subject->getModel()->setAnimate(true);
    m_subject->setupModel();
    QVERIFY(qTimeLineStartedCalled);
}

void Ut_StatusIndicatorAnimationView::testImageListInitialized()
{
    m_subject->getModel()->setValue("1 2");
    // Check that images are not loaded until they are used
    QCOMPARE(mThemePixmapPixmaps.size(), 0);
}

void Ut_StatusIndicatorAnimationView::testSetAnimationFrame()
{
    // Test that the latest image list is used
    controller->setGeometry(QRectF(0, 0, 50, 50));
    m_subject->getModel()->setValue("3 4");
    m_subject->getModel()->setValue("1 2");
    QPainter painter;

    // Check that correct images get loaded from the theme and drawn
    m_subject->setAnimationFrame(0);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(mThemePixmapPixmaps.size(), 1);
    QVERIFY(mThemePixmapPixmaps.key("1") != NULL);
    QCOMPARE(qPainterDrawPixmapPixmaps.size(), 1);
    QCOMPARE(qPainterDrawPixmapPixmaps.at(0), mThemePixmapPixmaps.key("1"));

    qPainterDrawPixmapPixmaps.clear();
    m_subject->setAnimationFrame(1);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(mThemePixmapPixmaps.size(), 2);
    QVERIFY(mThemePixmapPixmaps.key("2") != NULL);
    QCOMPARE(qPainterDrawPixmapPixmaps.size(), 1);
    QCOMPARE(qPainterDrawPixmapPixmaps.at(0), mThemePixmapPixmaps.key("2"));

    // Check that images are not loaded again if they are already loaded
    mThemePixmapPixmaps.clear();
    m_subject->setAnimationFrame(0);
    m_subject->callableDrawContents(&painter, NULL);
    m_subject->setAnimationFrame(1);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(mThemePixmapPixmaps.size(), 0);
}

void Ut_StatusIndicatorAnimationView::testSetAnimationFrameToInvalid()
{
    // First test painting when there is no image
    controller->setGeometry(QRectF(0, 0, 50, 50));
    m_subject->getModel()->setValue("");
    QPainter painter;

    // Set animation frame to smaller than allowed
    m_subject->setAnimationFrame(-2);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(qPainterDrawPixmapPixmaps.size(), 0);
    QCOMPARE(mThemePixmapPixmaps.size(), 0);

    // Set animation frame to larger than allowed
    qPainterDrawPixmapPixmaps.clear();
    m_subject->setAnimationFrame(3);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(qPainterDrawPixmapPixmaps.size(), 0);
    QCOMPARE(mThemePixmapPixmaps.size(), 0);

    // Test the same with a longer image list
    qPainterDrawPixmapPixmaps.clear();
    mThemePixmapPixmaps.clear();
    m_subject->getModel()->setValue("1 2");

    // Set animation frame to smaller than allowed
    m_subject->setAnimationFrame(-2);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(qPainterDrawPixmapPixmaps.size(), 1);
    QCOMPARE(qPainterDrawPixmapPixmaps.at(0), mThemePixmapPixmaps.key("1"));
    QCOMPARE(mThemePixmapPixmaps.size(), 1);
    QVERIFY(mThemePixmapPixmaps.key("1") != NULL);

    // Set animation frame to larger than allowed
    qPainterDrawPixmapPixmaps.clear();
    m_subject->setAnimationFrame(3);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(qPainterDrawPixmapPixmaps.size(), 1);
    QCOMPARE(qPainterDrawPixmapPixmaps.at(0), mThemePixmapPixmaps.key("2"));
    QCOMPARE(mThemePixmapPixmaps.size(), 2);
    QVERIFY(mThemePixmapPixmaps.key("2") != NULL);
}

void Ut_StatusIndicatorAnimationView::testSetAnimationDuration()
{
    m_subject->getModel()->setValue("1 2 3");
    m_subject->modifiableStyle()->setAnimationDuration(33);
    m_subject->executeStyleChanged();
    QCOMPARE(qTimeLineDuration, 33);
    QCOMPARE(qTimeLineFrameRange, qMakePair(0, 3));
    QCOMPARE(qTimeLineUpdateInterval, 11);
    m_subject->setFirstAnimationFrame(1);
    QCOMPARE(qTimeLineFrameRange, qMakePair(1, 3));
    QCOMPARE(qTimeLineUpdateInterval, 16);
}

void Ut_StatusIndicatorAnimationView::testChangingAnimate()
{
    QCOMPARE(m_subject->getModel()->animate(), false);

    // Since animate is false by default, resetting it to false shouldn't have any effect
    m_subject->getModel()->setAnimate(false);
    QCOMPARE(qTimeLineStartedCalled, false);
    QCOMPARE(qTimeLineStoppedCalled, false);

    m_subject->getModel()->setAnimate(true);
    QCOMPARE(qTimeLineStartedCalled, true);
    QCOMPARE(qTimeLineStoppedCalled, false);

    qTimeLineStartedCalled = false;

    // Setting the animate again to true shouldn't have any effect
    m_subject->getModel()->setAnimate(true);
    QCOMPARE(qTimeLineStartedCalled, false);
    QCOMPARE(qTimeLineStoppedCalled, false);

    m_subject->getModel()->setAnimate(false);
    QCOMPARE(qTimeLineStartedCalled, false);
    QCOMPARE(qTimeLineStoppedCalled, true);
}

void Ut_StatusIndicatorAnimationView::testPaintingWhenSizeIsZero()
{
    // Test that the latest image list is used
    m_subject->getModel()->setValue("1 2");
    QPainter painter;

    // Nothing should be loaded or drawn
    m_subject->setAnimationFrame(0);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(mThemePixmapPixmaps.size(), 0);
    QCOMPARE(qPainterDrawPixmapPixmaps.size(), 0);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorAnimationView)
