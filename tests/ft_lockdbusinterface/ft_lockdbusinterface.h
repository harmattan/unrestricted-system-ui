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
#ifndef FT_LOCKDBUSINTERFACE_H
#define FT_LOCKDBUSINTERFACE_H

#include <QtTest/QtTest>
#include <QObject>
#include <QDBusInterface>
#include <QDBusError>
#include "../ft_lockscreenui/xchecker.h"

class MApplication;
class MApplicationWindow;

class SingnalSink : public QObject 
{
    Q_OBJECT
public:
    SingnalSink ();

    void reset ();
    void debugPrint ();

public slots:
    void DBusMessagingFailure (QDBusError error);
    void screenLockReply (qint32 reply);

public:
    bool     m_ErrorCame;
    bool     m_ReplyCame;
    qint32   m_Reply;
};

/*!
 * Tests the lockscreenui showing/hiding by sending dbus messages to the
 * sysuid.
 */
class Ft_LockDBusInterface : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testEventEater ();
    void testLockScreenShowHide ();

private:
    void lockScreen ();
    void unLockScreen ();
    
    void showEventEater ();

    void checkLockIsVisible ();
    void checkLockIsInvisible ();
    void checkEaterIsVisible ();
    void checkEaterIsInvisible ();
private:
    XChecker             m_XChecker;
    SingnalSink          m_SignalSink;
    MApplicationWindow  *m_MainWindow;
    MApplication        *m_App;
    QDBusInterface      *m_DbusIf;
};

#endif

