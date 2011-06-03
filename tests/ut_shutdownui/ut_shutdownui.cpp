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
#include "shutdownwindowstyle.h"
#include "x11wrapper_modified_stub.h"

#include <MApplication>
#include <MWindow>
#include <MNotification>
#include <MStylableWidget>
#include <QGraphicsLinearLayout>

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

bool gTimerStarted;
void QTimer::start()
{
    gTimerStarted = true;
}

bool mWindowOrientationLocked = false;
void MWindow::setOrientationLocked(bool locked)
{
    mWindowOrientationLocked = locked;
}

bool mWindowOrientationAngleLocked = false;
void MWindow::setOrientationAngleLocked(bool locked)
{
    mWindowOrientationAngleLocked = locked;
}

QString mWindowOrientation;
void MWindow::setLandscapeOrientation()
{
    mWindowOrientation = "landscape";
}

void MWindow::setPortraitOrientation()
{
    mWindowOrientation = "portrait";
}

M::OrientationAngle mWindowOrientationAngle;
void MWindow::setOrientationAngle(M::OrientationAngle angle)
{
    mWindowOrientationAngle = angle;
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

    gTimerStarted = false;
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
    QCOMPARE(params.msec, 12000);
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

void Ut_ShutdownUI::testShowWindowWithEmptyStrings()
{
    // Check that calling showWindow() will realize the widget and show logo
    m_subject->showWindow("", "", 2000);

    QVERIFY(m_subject->realized);
    QVERIFY(m_subject->layout->itemAt(0) == m_subject->logo);
    QCOMPARE(gTimerStarted, false);
}

void Ut_ShutdownUI::testOrientationLocking_data()
{
    QTest::addColumn<QString>("lockedOrientation");
    QTest::addColumn<bool>("orientationLocked");
    QTest::addColumn<QString>("expectedOrientation");
    QTest::addColumn<int>("expectedOrientationAngle");

    QTest::newRow("No locking") << QString() << false << QString() << (int)M::Angle180;
    QTest::newRow("Locked to landscape") << QString("landscape") << true << QString("landscape") << (int)M::Angle0;
    QTest::newRow("Locked to portrait") << QString("portrait") << true << QString("portrait") << (int)M::Angle270;
    QTest::newRow("Locked to something else") << QString("unknown") << false << QString() << (int)M::Angle180;
}

void Ut_ShutdownUI::testOrientationLocking()
{
    QFETCH(QString, lockedOrientation);
    QFETCH(bool, orientationLocked);
    QFETCH(QString, expectedOrientation);
    QFETCH(int, expectedOrientationAngle);

    // Reset the stubs
    mWindowOrientationLocked = false;
    mWindowOrientation = QString();
    mWindowOrientationAngle = M::Angle180;

    // Set the style
    ShutdownWindowStyle *style = const_cast<ShutdownWindowStyle *>(static_cast<const ShutdownWindowStyle *>(MTheme::style("ShutdownWindowStyle", "", "", "", M::Landscape, NULL)));
    style->setLockedOrientation(lockedOrientation);

    // Create a new window
    delete m_subject;
    m_subject = new ShutdownUI;
    QCOMPARE(mWindowOrientationLocked, orientationLocked);
    QCOMPARE(mWindowOrientationAngleLocked, orientationLocked);
    QCOMPARE(mWindowOrientation, expectedOrientation);
    QCOMPARE(mWindowOrientationAngle, (M::OrientationAngle)expectedOrientationAngle);
}

QTEST_APPLESS_MAIN(Ut_ShutdownUI)
