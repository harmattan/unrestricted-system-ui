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

#include "ut_mcompositornotificationsink.h"

#include <QtTest/QtTest>
#include <MRemoteAction>
#include <MInfoBanner>
#include <MApplication>
#include <MScene>
#include <MSceneManager>
#include "mcompositornotificationsink.h"
#include "notificationwidgetparameterfactory.h"
#include "testnotificationparameters.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include <QSettings>
#include "sysuid_stub.h"
#include "notificationmanager.h"
#include "eventtypestore.h"
#include "genericnotificationparameterfactory.h"
#include <QSettings>
#include <MOnDisplayChangeEvent>

maemosec::storage::~storage()
{
}

// Mock notification manager (used by MCompositorNotificationSink)
MockNotificationManager::MockNotificationManager() :
    nextAvailableNotificationID(0)
{
}

uint MockNotificationManager::addNotification(uint, const NotificationParameters &parameters, uint groupId, int timeout)
{
    uint notificationId = nextAvailableNotificationID++;
    Notification notification = Notification(notificationId, groupId, 0, parameters, Notification::ApplicationEvent, timeout);
    notifications.append(notification);
    emit notificationUpdated(notification);
    return notificationId;
}

uint MockNotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId)
{
    return addNotification(notificationUserId, parameters, groupId, 1000);
}

bool MockNotificationManager::updateNotification(uint, uint notificationId, const NotificationParameters &parameters)
{
    for (int i = 0; i < notifications.count(); ++i) {
        if (notifications.at(i).notificationId() == notificationId) {
            Notification oldNotification = notifications.at(i);
            Notification newNotification = Notification(notificationId, oldNotification.groupId(), oldNotification.userId(), parameters, oldNotification.type(), 1000);
            notifications[i] = newNotification;
            emit notificationUpdated(newNotification);
            return true;
        }
    }
    return false;
}

bool MockNotificationManager::removeNotification(uint, uint notificationId)
{
    bool removed = false;
    for (int i = 0; i < notifications.count(); ++i) {
        if (notifications.at(i).notificationId() == notificationId) {
            notifications.removeAt(i);
            removed = true;
            break;
        }
    }

    if (removed) {
        emit notificationRemoved(notificationId);
    }

    return removed;
}

uint MockNotificationManager::addGroup(uint, const NotificationParameters &)
{
    return 0;
}

bool MockNotificationManager::updateGroup(uint, uint, const NotificationParameters &)
{
    return false;
}

bool MockNotificationManager::removeGroup(uint, uint)
{
    return false;
}

uint MockNotificationManager::notificationUserId()
{
    return 0;
}

QList< uint > MockNotificationManager::notificationIdList(uint /*notificationUserId*/)
{
    QList<uint> tmp;
    return tmp;
}

QList<Notification> MockNotificationManager::notificationList(uint)
{
    return QList<Notification>();
}

QList<NotificationGroup> MockNotificationManager::notificationGroupList(uint)
{
    return QList<NotificationGroup>();
}


// QWidget stubs (used by MCompositorNotificationSink)
bool Ut_WindowShown = false;
QWidget* notificationWindow = NULL;
void QWidget::setVisible(bool visible)
{
    Ut_WindowShown = visible;
    notificationWindow = this;
}

MSceneWindow *infoBanner = NULL;
bool Ut_DestroyWhenDonePolicy = false;
void MSceneManager::appearSceneWindow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy)
{
    infoBanner = sceneWindow;
    if(policy == MSceneWindow::DestroyWhenDone) {
        Ut_DestroyWhenDonePolicy = true;
    }
}

bool Ut_DisappearSceneWindow= false;
MSceneWindow *disappearingWindow= NULL;
void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    Ut_DisappearSceneWindow= true;
    disappearingWindow = sceneWindow;
}

// QTimer stubs (used by MCompositorNotificationSink)
bool Ut_TimerStarted = false;
void QTimer::start(int msec)
{
    Ut_TimerStarted = true;
    Q_UNUSED(msec);
}

// MSceneWindow stubs (used by MCompositorNotificationSink)
void MSceneWindow::disappear()
{
    emit disappeared();
}

//QWidget Stubs
QRegion maskRegionReceived(0,0,0,0,QRegion::Rectangle);
void QWidget::setMask(const QRegion & region)
{
    maskRegionReceived = region;
}

// Tests
void Ut_MCompositorNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mcompositornotificationsink";
    app = new MApplication(argc, &app_name);
}

void Ut_MCompositorNotificationSink::cleanupTestCase()
{
    delete app;
}

void Ut_MCompositorNotificationSink::init()
{
    notificationManager = new MockNotificationManager();
    sink = new MCompositorNotificationSink();
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), sink, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(notificationUpdated(Notification)), sink, SLOT(addNotification(Notification)));
    Ut_WindowShown = false;
    notificationWindow = NULL;
    infoBanner = NULL;
    Ut_DestroyWhenDonePolicy = false;
    Ut_TimerStarted = false;
    Ut_DisappearSceneWindow = false;
    disappearingWindow = NULL;
    maskRegionReceived = QRegion();
}

void Ut_MCompositorNotificationSink::cleanup()
{
    delete sink;
    delete notificationManager;
}

void Ut_MCompositorNotificationSink::testAddNotification()
{
    QSignalSpy spy(sink, SIGNAL(notificationAdded(const Notification&)));

    // Create a notification
    TestNotificationParameters parameters0("icon0", "summary0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);

    // Check that a MWindow was created and shown(window->show() called)
    QCOMPARE(Ut_WindowShown, true);

    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);

    // Check that a MInfoBanner was created with the given parameters
    QCOMPARE(Ut_DestroyWhenDonePolicy, true);

    MInfoBanner *banner = static_cast<MInfoBanner*>(infoBanner);
    QCOMPARE(banner->bannerType(), MInfoBanner::Event);
    QCOMPARE(banner->iconID(), QString("buttonicon0"));
    QCOMPARE(banner->bodyText(), QString("<p><b>summary0</b></p><p>body0</p>"));
    QCOMPARE(banner->imageID(), QString("icon0"));

    // check that the timeout timer was started
    QCOMPARE(Ut_TimerStarted, true);

    // check that the transfer notification signal was emitted
    QCOMPARE(spy.count(), 1);
}

void Ut_MCompositorNotificationSink::testUpdateNotification()
{
    // Create two notifications
    TestNotificationParameters parameters0("icon0", "summary0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);
    MInfoBanner *banner1 = static_cast<MInfoBanner*>(infoBanner);

    TestNotificationParameters parameters1("icon1", "summary1", "body1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    notificationWindow->hide();
    QApplication::sendEvent(notificationWindow,event);
    MInfoBanner *banner2 = static_cast<MInfoBanner*>(infoBanner);

    // Update the second notification
    TestNotificationParameters parametersX("iconX", "summaryX", "bodyX", "buttoniconX", "contentX X X X");
    notificationManager->updateNotification(0, id, parametersX);

    QCOMPARE(banner2->imageID(), QString("iconX"));
    QCOMPARE(banner2->bodyText(), QString("<p><b>summaryX</b></p><p>bodyX</p>"));
    QCOMPARE(banner2->iconID(), QString("buttoniconX"));

    // Make sure the fist notification banner has still the same content
    QCOMPARE(banner1->imageID(), QString("icon0"));
    QCOMPARE(banner1->bodyText(), QString("<p><b>summary0</b></p><p>body0</p>"));
    QCOMPARE(banner1->iconID(), QString("buttonicon0"));
}

void Ut_MCompositorNotificationSink::testRemoveNotification()
{
    // Create three notifications
    TestNotificationParameters parameters0("icon0", "summary0", "body0", "buttonicon0", "content0 0 0 0");
    parameters0.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters0, 0);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);

    TestNotificationParameters parameters1("icon1", "summary1", "body1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    notificationWindow->hide();
    QApplication::sendEvent(notificationWindow,event);
    MInfoBanner *banner = static_cast<MInfoBanner*>(infoBanner);

    TestNotificationParameters parameters2("icon2", "summary2", "body2", "buttonicon2", "");
    notificationManager->addNotification(0, parameters2);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    notificationWindow->hide();
    QApplication::sendEvent(notificationWindow,event);

    // Remove the second one
    notificationManager->removeNotification(0, id);
    // Check that the window was removed
    QCOMPARE(banner, disappearingWindow);
}

void Ut_MCompositorNotificationSink::testTimeout()
{
    TestNotificationParameters parameters0("icon0", "summary0", "body0", "buttonicon0", "content0 0 0 0");
    uint notificationId = notificationManager->addNotification(0, parameters0);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);

    connect(this, SIGNAL(timeout()), sink, SLOT(timeout()));
    setProperty("notificationId", notificationId);
    emit(timeout());
    // Check that the window disappeared
    QCOMPARE(infoBanner, disappearingWindow);
    QCOMPARE(Ut_DisappearSceneWindow, true);
}

void Ut_MCompositorNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create notification
    TestNotificationParameters parameters("icon0", "summary0", "body0", "buttonicon0", "content0 0 0 0");
    sink->setApplicationEventsEnabled(false);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(Ut_WindowShown, false);

    sink->setApplicationEventsEnabled(true);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(Ut_WindowShown, true);
}

void Ut_MCompositorNotificationSink::testWhenSinkDisableTrueNoBannerCreated()
{
    connect(this, SIGNAL(statusIndictorMenuVisibilityChanged(bool)), sink, SLOT(setDisabled(bool)));
    emit statusIndictorMenuVisibilityChanged(true);
    // Create notification
    TestNotificationParameters parameters("icon0", "summary0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters);
    QCOMPARE(Ut_WindowShown, false);
    emit statusIndictorMenuVisibilityChanged(false);
    notificationManager->addNotification(0, parameters);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);
    QCOMPARE(Ut_WindowShown, true);
}

QTEST_APPLESS_MAIN(Ut_MCompositorNotificationSink)
