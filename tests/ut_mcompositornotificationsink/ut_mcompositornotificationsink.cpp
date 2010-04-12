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

#define NUMBER_OF_WINDOWS 2

static QSettings *settings;

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

// QGraphicsWidget stubs (used by MCompositorNotificationSink)
void QGraphicsWidget::addAction(QAction *action)
{
    Ut_MCompositorNotificationSink::actions[this].append(action);

    MRemoteAction *dra = dynamic_cast<MRemoteAction *>(action);
    if (dra != NULL) {
        Ut_MCompositorNotificationSink::contents.append(dra->toString());
    }
}

void QGraphicsWidget::removeAction(QAction *action)
{
    Ut_MCompositorNotificationSink::actions[this].removeAll(action);

    MRemoteAction *dra = dynamic_cast<MRemoteAction *>(action);
    if (dra != NULL) {
        Ut_MCompositorNotificationSink::contents.removeAll(dra->toString());
    }
}

QList<QAction *> QGraphicsWidget::actions() const
{
    return Ut_MCompositorNotificationSink::actions[this];
}

// QWidget stubs (used by MCompositorNotificationSink)
void QWidget::setVisible(bool)
{
}

void QWidget::setFixedSize(int w, int h)
{
    Ut_MCompositorNotificationSink::lastFixedWidth = w;
    Ut_MCompositorNotificationSink::lastFixedHeight = h;
}

// QTimer stubs (used by MCompositorNotificationSink)
void QTimer::start(int msec)
{
    Ut_MCompositorNotificationSink::lastTimeout = msec;

    if (msec <= 1) {
        emit timeout();
    }
}

// QGraphicsScene stubs (used by MCompositorNotificationSink)
void QGraphicsScene::addItem(QGraphicsItem *item)
{
    MInfoBanner *n = dynamic_cast<MInfoBanner *>(item);
    if (n != NULL) {
        Ut_MCompositorNotificationSink::types.append(n->bannerType());
        Ut_MCompositorNotificationSink::icons.append(n->model()->imageID());
        Ut_MCompositorNotificationSink::bodies.append(n->model()->bodyText());
        Ut_MCompositorNotificationSink::buttonIcons.append(n->model()->iconID());
        Ut_MCompositorNotificationSink::notifications.append(n);
    }
}

void QGraphicsScene::removeItem(QGraphicsItem *item)
{
    MInfoBanner *n = dynamic_cast<MInfoBanner *>(item);
    if (n != NULL) {
        int index = Ut_MCompositorNotificationSink::notifications.indexOf(n);

        Ut_MCompositorNotificationSink::types.removeAt(index);
        Ut_MCompositorNotificationSink::icons.removeAt(index);
        Ut_MCompositorNotificationSink::bodies.removeAt(index);
        Ut_MCompositorNotificationSink::contents.removeAt(index);
        Ut_MCompositorNotificationSink::actions[n].clear();
        Ut_MCompositorNotificationSink::buttonIcons.removeAt(index);
        Ut_MCompositorNotificationSink::notifications.removeAt(index);
    }
}

// QGraphicsView stubs (used by MCompositorNotificationSink)
void QGraphicsView::setTransform(const QTransform &matrix, bool)
{
    Ut_MCompositorNotificationSink::lastTransform = matrix;
}

// MSceneWindow stubs (used by MCompositorNotificationSink)
void MSceneWindow::disappear()
{
    emit disappeared();
}

// MInfoBanner stubs (used by MCompositorNotificationSink)
void MInfoBanner::setImageID(const QString &image)
{
    int index = Ut_MCompositorNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_MCompositorNotificationSink::icons.replace(index, image);
    }
    model()->setImageID(image);
}

void MInfoBanner::setBodyText(const QString &body)
{
    int index = Ut_MCompositorNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_MCompositorNotificationSink::bodies.replace(index, body);
    }
    model()->setBodyText(body);
}

void MInfoBanner::setIconID(const QString &iconId)
{
    int index = Ut_MCompositorNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_MCompositorNotificationSink::buttonIcons.replace(index, iconId);
    }
    model()->setIconID(iconId);
}

QList<MInfoBanner::BannerType> Ut_MCompositorNotificationSink::types;
QList<QString> Ut_MCompositorNotificationSink::icons;
QList<QString> Ut_MCompositorNotificationSink::bodies;
QList<QString> Ut_MCompositorNotificationSink::buttonIcons;
QList<QString> Ut_MCompositorNotificationSink::contents;
QHash<const QGraphicsWidget *, QList<QAction *> > Ut_MCompositorNotificationSink::actions;
QList<MInfoBanner *> Ut_MCompositorNotificationSink::notifications;
int Ut_MCompositorNotificationSink::lastTimeout;
int Ut_MCompositorNotificationSink::lastFixedWidth;
int Ut_MCompositorNotificationSink::lastFixedHeight;
QTransform Ut_MCompositorNotificationSink::lastTransform;

// Tests
void Ut_MCompositorNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mcompositornotificationsink";
    app = new MApplication(argc, &app_name);
    sysuid = new Sysuid();
    settings = new QSettings();
    gSysuidStub->stubSetReturnValue("sysuid",sysuid);
}

void Ut_MCompositorNotificationSink::cleanupTestCase()
{
    delete settings;
    delete sysuid;
    delete app;
}

void Ut_MCompositorNotificationSink::init()
{
    notificationManager = new MockNotificationManager();
    sink = new MCompositorNotificationSink();
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), sink, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(notificationUpdated(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(orientationChangeFinished(M::Orientation)), sink, SLOT(rotateInfoBanners(M::Orientation)));
    lastTimeout = -1;
    types.clear();
    icons.clear();
    bodies.clear();
    buttonIcons.clear();
    contents.clear();
    actions.clear();
    notifications.clear();
    lastFixedWidth = -1;
    lastFixedHeight = -1;
    lastTransform = QTransform();
    gSysuidStub->stubSetReturnValue("orientation", M::Landscape);
    gSysuidStub->stubSetReturnValue("orientationAngle", M::Angle0);
}

void Ut_MCompositorNotificationSink::cleanup()
{
    delete sink;
    delete notificationManager;
}

void Ut_MCompositorNotificationSink::testAddNotification()
{
    // Create three notifications - two with a content link and one without
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    parameters0.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters0, 0);
    notificationManager->addNotification(0, parameters1);

    // Check that two MInfoBanners were created
    QCOMPARE(notifications.count(), 2);

    // Make sure the view size and rotation was set correctly
    MInfoBanner infoBanner1(types[1]);
    QTransform transform1;
    transform1.rotate(Sysuid::sysuid()->orientationAngle());
    QCOMPARE(lastFixedWidth, (int)infoBanner1.preferredWidth());
    QCOMPARE(lastFixedHeight, (int)infoBanner1.preferredHeight());
    QCOMPARE(lastTransform, transform1);

    gSysuidStub->stubSetReturnValue("orientation", M::Portrait);
    gSysuidStub->stubSetReturnValue("orientationAgle", M::Angle90);
    // Create a notification without a content link
    TestNotificationParameters parameters3("icon3", "body3", "buttonicon3", "");
    notificationManager->addNotification(0, parameters3);

    // Check that a MInfoBanner was created with the given parameters
    QCOMPARE(types.length(), 3);
    QCOMPARE(types[2], MInfoBanner::Event);
    QCOMPARE(icons.length(), 3);
    QCOMPARE(icons[2], QString("icon3"));
    QCOMPARE(bodies.length(), 3);
    QCOMPARE(bodies[2], QString("body3"));
    QCOMPARE(buttonIcons.length(), 3);
    QCOMPARE(buttonIcons[2], QString("buttonicon3"));
    QCOMPARE(contents.length(), 2);

    // Make sure the view size and rotation was set correctly
    MInfoBanner infoBanner2(types[2]);
    QTransform transform2;
    transform2.rotate(Sysuid::sysuid()->orientationAngle());
    QCOMPARE(lastFixedWidth, (int)infoBanner2.preferredHeight());
    QCOMPARE(lastFixedHeight, (int)infoBanner2.preferredWidth());
    QCOMPARE(lastTransform, transform2);
}

void Ut_MCompositorNotificationSink::testUpdateNotification()
{
    // Create two notifications
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    notificationManager->addNotification(0, parameters0);
    uint id = notificationManager->addNotification(0, parameters1);

    // Update the second notification
    TestNotificationParameters parametersX("iconX", "bodyX", "buttoniconX", "contentX X X X");
    notificationManager->updateNotification(0, id, parametersX);

    // Check that two MInfoBanners were created with the given parameters
    QCOMPARE(types.length(), 2);
    QCOMPARE(types[0], MInfoBanner::Event);
    QCOMPARE(types[1], MInfoBanner::Event);
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("iconX"));
    QCOMPARE(bodies.length(), 2);
    QCOMPARE(bodies[0], QString("body0"));
    QCOMPARE(bodies[1], QString("bodyX"));
    QCOMPARE(buttonIcons.length(), 2);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(buttonIcons[1], QString("buttoniconX"));
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[0], QString("content0 0 0 0"));
    QCOMPARE(contents[1], QString("contentX X X X"));
}

void Ut_MCompositorNotificationSink::testRemoveNotification()
{
    // Create three notifications
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    TestNotificationParameters parameters2("icon2", "body2", "buttonicon2", "");
    parameters0.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters0, 0);
    uint id = notificationManager->addNotification(0, parameters1);
    notificationManager->addNotification(0, parameters2);

    // Remove the second one
    notificationManager->removeNotification(0, id);

    // Check that two MInfoBanners exist with the given icon parameters (to identify them)
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("icon2"));

    // Recreate the second notification and create an additional one
    notificationManager->addNotification(0, parameters1);
    TestNotificationParameters parameters4("icon3", "body3", "buttonicon3", "");
    parameters4.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters4, 0);
    QCOMPARE(icons.length(), 4);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("icon2"));
    QCOMPARE(icons[2], QString("icon1"));
    QCOMPARE(icons[3], QString("icon3"));
}

void Ut_MCompositorNotificationSink::testTimeout()
{
    // Create a notification with a timeout of 1000 milliseconds
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);

    // Check that the timeout was set
    QCOMPARE(lastTimeout, 1000);

    // The notification should be created
    QCOMPARE(notifications.length(), 1);

    // Create a notification with a timeout of 0 milliseconds
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    notificationManager->addNotification(0, parameters1, 0, 0);

    // Check that the timeout was set
    QCOMPARE(lastTimeout, 0);

    // The notification should be immediately destroyed
    QCOMPARE(notifications.length(), 1);
}

void Ut_MCompositorNotificationSink::testOrientationChanged()
{
    // Make sure landscape is activated
    emit orientationChangeFinished(M::Landscape);

    // Create a notification
    TestNotificationParameters parameters("icon0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters);

    // The notification should be created
    QCOMPARE(types.length(), 1);
    QCOMPARE(icons.length(), 1);
    QCOMPARE(bodies.length(), 1);
    QCOMPARE(buttonIcons.length(), 1);

    // Make sure the view size and rotation was set correctly
    MInfoBanner infoBanner(types[0]);
    QTransform transform;
    transform.rotate(Sysuid::sysuid()->orientationAngle());
    QCOMPARE(lastFixedWidth, (int)infoBanner.preferredWidth());
    QCOMPARE(lastFixedHeight, (int)infoBanner.preferredHeight());
    QCOMPARE(lastTransform, transform);

    // Rotate the screen
    gSysuidStub->stubSetReturnValue("orientation", M::Portrait);
    gSysuidStub->stubSetReturnValue("orientationAngle", M::Angle90);
    emit orientationChangeFinished(M::Portrait);

    // Make sure the view size and rotation was set correctly
    QTransform transform2;
    transform2.rotate(Sysuid::sysuid()->orientationAngle());
    QCOMPARE((int)infoBanner.preferredHeight(), lastFixedWidth);
    QCOMPARE((int)infoBanner.preferredWidth(), lastFixedHeight);

    QCOMPARE(lastTransform, transform2);
}

void Ut_MCompositorNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create notification
    TestNotificationParameters parameters("icon0", "body0", "buttonicon0", "content0 0 0 0");
    sink->setApplicationEventsEnabled(true);
    notificationManager->addNotification(0, parameters);
    // Check that notification was added when application events enabled
    QCOMPARE(notifications.count(), 1);

    sink->setApplicationEventsEnabled(false);
    notificationManager->addNotification(0, parameters);
    // Check that notification was NOT added when application events are NOT enabled
    QCOMPARE(notifications.count(), 1);
}

void Ut_MCompositorNotificationSink::testWhenSinkDisableTrueNoBannerCreated()
{
    connect(this, SIGNAL(statusIndictorMenuVisibilityChanged(bool)), sink, SLOT(setDisabled(bool)));
    emit statusIndictorMenuVisibilityChanged(true);
    // Create notification
    TestNotificationParameters parameters("icon0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters);
    // Check that notification banner was not created when status indicator menu is visible
    QCOMPARE(notifications.count(), 0);
    emit statusIndictorMenuVisibilityChanged(false);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(notifications.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_MCompositorNotificationSink)
