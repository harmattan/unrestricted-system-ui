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
#include <MApplication>
#include <MMessageBox>
#include <memnotify/memory_notification.hpp>
#include "ut_lowmemorynotifier.h"
#include "lowmemorynotifier.h"

bool memoryNotificationValidCalled = false;
bool MEMNOTIFY::MemoryNotification::valid() const
{
    memoryNotificationValidCalled = true;
    return true;
}

bool memoryNotificationEnableCalled = false;
bool MEMNOTIFY::MemoryNotification::enable()
{
    memoryNotificationEnableCalled = true;
    return true;
}

bool memoryNotificationPollCalled = false;
bool MEMNOTIFY::MemoryNotification::poll()
{
    memoryNotificationPollCalled = true;
    return true;
}

bool mDialogAppearCalled = false;
void MDialog::appear(MSceneWindow::DeletionPolicy)
{
    mDialogAppearCalled = true;
}

bool mSceneWindowDisappearCalled = false;
void MSceneWindow::disappear()
{
    mSceneWindowDisappearCalled = true;
}

void Ut_LowMemoryNotifier::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_lowmemorynotifier";
    app = new MApplication(argc, &app_name);
}

void Ut_LowMemoryNotifier::cleanupTestCase()
{
    delete app;
}

void Ut_LowMemoryNotifier::init()
{
    m_subject = new LowMemoryNotifier();
}

void Ut_LowMemoryNotifier::cleanup()
{
    delete m_subject;
    memoryNotificationValidCalled = false;
    memoryNotificationEnableCalled = false;
    memoryNotificationPollCalled = false;
    mDialogAppearCalled = false;
    mSceneWindowDisappearCalled = false;
}

void Ut_LowMemoryNotifier::testInitialization()
{
    QCOMPARE(memoryNotificationValidCalled, true);
    QCOMPARE(memoryNotificationEnableCalled, true);
    QCOMPARE(memoryNotificationPollCalled, true);
    QCOMPARE(disconnect(&m_subject->memoryNotification, SIGNAL(notified(const QString&, const bool)), m_subject, SLOT(handleLowMemoryNotification(const QString&, const bool))), true);
    QCOMPARE(m_subject->messageBox->isSystem(), true);
    QCOMPARE(m_subject->messageBox->iconId(), QString("icon-m-bootloader-warning"));
}

void Ut_LowMemoryNotifier::testHandleLowMemoryNotification()
{
    m_subject->handleLowMemoryNotification(QString(), true);
    QCOMPARE(mDialogAppearCalled, true);

    m_subject->handleLowMemoryNotification(QString(), false);
    QCOMPARE(mSceneWindowDisappearCalled, true);
}

QTEST_APPLESS_MAIN(Ut_LowMemoryNotifier)
