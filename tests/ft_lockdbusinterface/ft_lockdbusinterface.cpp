/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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

#include "ft_lockdbusinterface.h"
#include "sysuid_stub.h"

#include <QList>
#include <QVariant>
#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MTheme>
#include <qmlocks.h>

using namespace Maemo;

#define DEBUG
#include "../../src/debug.h"

/*
 * The same should be used in the sysuid, in the mce and in the tests.
 */
enum {
    TkLockModeNone,      // Unused, maybe deprecated
    TkLockModeEnable,    // Show the lock UI in lock mode
    TkLockModeHelp,      // Unused, deprecated
    TkLockModeSelect,    // Unused, deprecated
    TkLockModeOneInput,  // Turn the event eater on
    TkLockEnableVisual   // Show unlock ui
} TkLockMode;

/*
 * These maybe should be implemented around here to check if we got the answer.
 */
static const QString answerService = "service";
static const QString answerPath = "path";
static const QString answerinterface = "interface";
static const QString answerMethod = "method";

/*
 * We will wait this much for the DBus answer.
 */
static int DBusDelay = 400;

/*
 * This much we wait between the tests.
 */
static int DelayBetweenTests = 1000;

/*********************************************************************************
 * SingnalSink implementation.
 */
SingnalSink::SingnalSink() :
    m_ErrorCame (false),
    m_ReplyCame (false),
    m_Reply (-1)
{
}

void 
SingnalSink::DBusMessagingFailure (
        QDBusError error)
{
    SYS_DEBUG ("ERROR: %s: %s", SYS_STR(error.name()), SYS_STR(error.message()));
    m_ErrorCame = true;
}

void
SingnalSink::screenLockReply (
        qint32 reply)
{
    SYS_DEBUG ("*** reply = %d", reply);
    m_ReplyCame = true;
    m_Reply = reply;
}

void
SingnalSink::reset ()
{
    m_ErrorCame = false;
    m_ReplyCame = false;
    m_Reply     = -1;
}

void
SingnalSink::debugPrint ()
{
    SYS_DEBUG ("*** m_ErrorCame   = %s", SYS_BOOL(m_ErrorCame));
    SYS_DEBUG ("*** m_ReplyCame   = %s", SYS_BOOL(m_ReplyCame));
    SYS_DEBUG ("*** m_Reply       = %d", m_Reply);
}

/*********************************************************************************
 * Ft_LockDBusInterface implementation.
 */
void Ft_LockDBusInterface::init()
{
}

void Ft_LockDBusInterface::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ft_lockdbusinterface",
    NULL };

const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

void Ft_LockDBusInterface::initTestCase()
{
    m_MainWindow = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
    MTheme::loadCSS (styleDir + "unlockscreen.css");

    m_DbusIf = new QDBusInterface (
            "com.nokia.system_ui", 
            "/com/nokia/system_ui/request", 
            "com.nokia.system_ui.request", 
            QDBusConnection::systemBus ());

    SYS_DEBUG ("*** isValid()    = %s", SYS_BOOL(m_DbusIf->isValid()));
    SYS_DEBUG ("*** interface()  = %s", SYS_STR(m_DbusIf->interface()));
    SYS_DEBUG ("*** service()    = %s", SYS_STR(m_DbusIf->service()));
    SYS_DEBUG ("*** path()       = %s", SYS_STR(m_DbusIf->path()));

    if (!m_DbusIf->isValid()) {
        QDBusError error = m_DbusIf->lastError ();
        SYS_DEBUG ("ERROR: %s: %s", 
                SYS_STR(error.name()),
                SYS_STR(error.message()));
    }

    QVERIFY (m_DbusIf->isValid());
}

void
Ft_LockDBusInterface::testLockScreen ()
{
    lockScreen ();
    checkLockIsVisible ();

    unLockScreen ();
    checkLockIsInvisible ();
}



void 
Ft_LockDBusInterface::cleanupTestCase()
{
    delete m_DbusIf;
    if (m_MainWindow)
        delete m_MainWindow; 
    delete m_App;
}

/*********************************************************************************
 * Low level methods for the Ft_LockDBusInterface class.
 */
/*
dbus-send --print-reply --system --dest=com.nokia.system_ui /com/nokia/system_ui/request com.nokia.system_ui.request.tklock_open string: string: string: string: uint32:5  boolean:false boolean:false
*/
void
Ft_LockDBusInterface::lockScreen ()
{
    QList <QVariant> arguments;

    arguments << 
        QVariant (answerService) <<
        QVariant (answerPath) << 
        QVariant (answerinterface) <<
        QVariant (answerMethod) <<
        QVariant ((quint32) TkLockModeEnable) << 
        QVariant (true) << //silent 
        QVariant (true); // flicker

    /*
     * Locking the screen through the dbus interface.
     */
    SYS_DEBUG ("Sending tklock_open...");
    m_SignalSink.reset();
    m_DbusIf->callWithCallback (
            QString ("tklock_open"), arguments, &m_SignalSink,
            SLOT (screenLockReply(qint32)),
            SLOT (DBusMessagingFailure (QDBusError)));

    QTest::qWait (DBusDelay);

    m_SignalSink.debugPrint();
    QVERIFY (!m_SignalSink.m_ErrorCame);
    QVERIFY (m_SignalSink.m_ReplyCame);
    QVERIFY (m_SignalSink.m_Reply == 1);

    QTest::qWait (DelayBetweenTests);
}

void
Ft_LockDBusInterface::unLockScreen ()
{
    QList <QVariant> arguments;

    arguments << 
        QVariant (QVariant(true));

    /*
     * Unlocking the screen through the dbus interface.
     */
    SYS_DEBUG ("Sending tklock_close...");
    m_SignalSink.reset();
    m_DbusIf->callWithCallback (
            QString ("tklock_close"), arguments, &m_SignalSink,
            SLOT (screenLockReply(qint32)),
            SLOT (DBusMessagingFailure (QDBusError)));

    QTest::qWait (DBusDelay);

    m_SignalSink.debugPrint();
    QVERIFY (!m_SignalSink.m_ErrorCame);
    QVERIFY (m_SignalSink.m_ReplyCame);
    QVERIFY (m_SignalSink.m_Reply == 1);

    QTest::qWait (DelayBetweenTests);
}


void 
Ft_LockDBusInterface::checkLockIsVisible ()
{
    bool windowVisible;
        
    windowVisible = m_XChecker.checkWindow ("sysuid", XChecker::CheckIsVisible);
    QVERIFY (windowVisible);
}

void 
Ft_LockDBusInterface::checkLockIsInvisible ()
{
    bool windowVisible;
        
    windowVisible = m_XChecker.checkWindow ("sysuid", XChecker::CheckIsInvisible);
    QVERIFY (windowVisible);
}

QTEST_APPLESS_MAIN(Ft_LockDBusInterface)

