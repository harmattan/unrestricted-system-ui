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
#include "ut_statusindicatoriconview.h"
#include "statusindicatoriconview.h"
#include "statusindicator.h"
#include "inputmethodstatusindicatoradaptor_stub.h"

class TestStyleContainer : public MStyleContainer
{
public:
    static QString getCurrentMode(TestStyleContainer *styleContainer) { return styleContainer->currentMode(); }
};

// MTheme stubs
QString mThemePixmapId;
QSize mThemePixmapSize;
const QPixmap *mThemePixmapPixmap;
const QPixmap *MTheme::pixmap(const QString &id, const QSize &size)
{
    mThemePixmapId = id;
    mThemePixmapSize = size;
    return mThemePixmapPixmap;
}

const QPixmap *mThemeReleasePixmapPixmap;
void MTheme::releasePixmap(const QPixmap *pixmap)
{
    mThemeReleasePixmapPixmap = pixmap;
}

// QPainter stubs
QPointF qPainterDrawPixmapPoint;
const QPixmap *qPainterDrawPixmapPixmap;
void QPainter::drawPixmap(const QPointF &point, const QPixmap &pixmap)
{
    qPainterDrawPixmapPoint = point;
    qPainterDrawPixmapPixmap = &pixmap;
}

void Ut_StatusIndicatorIconView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_statusindicatoriconview" };
    app = new MApplication(argc, app_name);
}

void Ut_StatusIndicatorIconView::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorIconView::init()
{
    mThemePixmapId.clear();
    mThemePixmapSize = QSize();
    mThemePixmapPixmap = NULL;
    mThemeReleasePixmapPixmap = NULL;
    qPainterDrawPixmapPoint = QPointF(-1, -1);
    qPainterDrawPixmapPixmap = NULL;
    controller = new StatusIndicator;
    m_subject = new StatusIndicatorIconView(controller);
    m_subject->setModel(controller->model());
}

void Ut_StatusIndicatorIconView::cleanup()
{
    delete m_subject;
    delete controller;
}

void Ut_StatusIndicatorIconView::testSetupModel()
{
    QString pixmapId("test");
    m_subject->model()->setValue(pixmapId);
    m_subject->setupModel();
    QCOMPARE(mThemePixmapId, pixmapId);
}

void Ut_StatusIndicatorIconView::testUpdateData()
{
    QList<const char *> modifications;
    modifications << StatusIndicatorModel::Value;

    // Test that setting a pixmap ID loads the pixmap and sets the mode to icon
    QString pixmapId("test");
    m_subject->model()->setValue(pixmapId);
    m_subject->updateData(modifications);
    QCOMPARE(mThemePixmapId, pixmapId);
    QCOMPARE(TestStyleContainer::getCurrentMode(reinterpret_cast<TestStyleContainer *>(&m_subject->style())), QString("icon"));

    // Test that setting an empty pixmap ID releases the pixmap and sets the mode to default
    pixmapId.clear();
    mThemePixmapId.clear();
    m_subject->model()->setValue(pixmapId);
    m_subject->updateData(modifications);
    QCOMPARE(mThemePixmapId, pixmapId);
    QCOMPARE(mThemeReleasePixmapPixmap, mThemePixmapPixmap);
    QCOMPARE(TestStyleContainer::getCurrentMode(reinterpret_cast<TestStyleContainer *>(&m_subject->style())), QString());
}

void Ut_StatusIndicatorIconView::testDrawContents()
{
    QPainter painter;

    // Drawing without a pixmap should do nothing
    m_subject->drawContents(&painter, NULL);
    QCOMPARE(qPainterDrawPixmapPoint, QPointF(-1, -1));
    QCOMPARE(qPainterDrawPixmapPixmap, (QPixmap *)NULL);

    QPixmap pixmap(50, 50);
    QString pixmapId("test");
    mThemePixmapPixmap = &pixmap;
    m_subject->model()->setValue(pixmapId);
    m_subject->setupModel();
    m_subject->drawContents(&painter, NULL);
    QCOMPARE(qPainterDrawPixmapPoint, QPointF());
    QCOMPARE(qPainterDrawPixmapPixmap, mThemePixmapPixmap);
}


QTEST_APPLESS_MAIN(Ut_StatusIndicatorIconView)
