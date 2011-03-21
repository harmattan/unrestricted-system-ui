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
#include "ut_shutdownui.h"
#include "shutdownui.h"
#include "x11wrapper_modified_stub.h"

#include <MApplication>
#include <MWindow>
#include <MNotification>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
using namespace MeeGo;
#endif

#ifdef HAVE_QMSYSTEM
static bool dimmingShouldSucceed = true;

bool QmDisplayState::set(DisplayState)
{
    return dimmingShouldSucceed;
}
#endif

/******************************************************************************
 * The stub for MFeedback class.
 */
#include <MFeedback>

static QString nameOfLastFeedback;

void MFeedback::play() const
{
    nameOfLastFeedback = name();
}

/******************************************************************************
 * Stubbing the QWidget, so the UI will not be really shown.
 */
void QWidget::setVisible(bool)
{
}

struct QTimerSingleShotParams
{
    int msec;
    QObject *receiver;
    QString member;
};
QHash<QObject*, QTimerSingleShotParams> qTimerSingleShotParams;
void QTimer::singleShot(int msec, QObject *receiver, const char * member)
{
    QTimerSingleShotParams params;
    params.msec = msec;
    params.receiver = receiver;
    params.member = member;
    qTimerSingleShotParams[receiver] = params;
}

void Ut_ShutdownUI::initTestCase()
{
    static int argc = 1;
    static char *argv[] = { (char *) "./ut_shutdownui", NULL };
    app = new MApplication(argc, argv);
    app->setQuitOnLastWindowClosed(false);
}

void Ut_ShutdownUI::cleanupTestCase()
{
    delete app;
}

void Ut_ShutdownUI::init()
{
    m_subject = new ShutdownUI;
}

void Ut_ShutdownUI::cleanup()
{
    delete m_subject;
    qTimerSingleShotParams.clear();
}

void Ut_ShutdownUI::testInitialization()
{
    // Check that the constructor calls realize() with a single shot timer
    QVERIFY(qTimerSingleShotParams.contains(m_subject));
    const QTimerSingleShotParams &params(qTimerSingleShotParams.value(m_subject));
    QCOMPARE(params.msec, 5000);
    QCOMPARE(params.receiver, m_subject);
    QString timedSlotName(params.member);
    QVERIFY2(timedSlotName.endsWith(QString("realize()")), qPrintable(QString("Actual timed slot name was: ") + timedSlotName));
}

void Ut_ShutdownUI::testRealize()
{
    // Check that calling realize() will realize the widget
    m_subject->realize();
    QVERIFY (m_subject->realized);
    QVERIFY (m_subject->feedback);
    QVERIFY (m_subject->label1);
    QVERIFY (m_subject->label2);
    QVERIFY (m_subject->logo);
    QVERIFY (m_subject->sceneWindow);
}

void Ut_ShutdownUI::testShowWindow()
{
    // Check that calling showWindow() will realize the widget
    m_subject->showWindow("text1", "text2", 2000);

    QVERIFY(m_subject->realized);

    // Swiping should be disabled for this window
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XChangeProperty").parameter<Window>(1), m_subject->winId());
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XChangeProperty").parameter<Atom>(2), X11Wrapper::XInternAtom(0, "_MEEGOTOUCH_CUSTOM_REGION", 0));

    unsigned int region[] = { m_subject->rect().x(), m_subject->rect().y(), m_subject->rect().width(), m_subject->rect().height() };
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XChangeProperty").parameter<QByteArray>(6), QByteArray(reinterpret_cast<const char *>(&region[0]), sizeof(region)));
}

QTEST_APPLESS_MAIN(Ut_ShutdownUI)

