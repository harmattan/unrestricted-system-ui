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

#include "ut_notificationareasink.h"
#include "notificationareasink.h"

#include <QtTest/QtTest>
#include <MRemoteAction>
#include <MApplication>
#include <MGConfItem>
#include "testnotificationparameters.h"
#include "notification.h"

static QSettings *settings;
// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// MGConfItem stubs
bool gMGConfPrivateNotificationValue;
QVariant MGConfItem::value() const
{
    if(this->key() == "/desktop/meego/privacy/private_lockscreen_notifications") {
        return QVariant(gMGConfPrivateNotificationValue);
    }

    return QVariant();
}

// MRemoteAction stubs (used by NotificationAreaSink)
MRemoteAction::MRemoteAction(const QString &action, QObject *parent) : MAction(parent)
{
    Ut_NotificationAreaSink::contents.append(action);
}

// MBanner stubs (used by NotificationAreaSink)
MBanner::~MBanner()
{
    Ut_NotificationAreaSink::destroyedNotifications.append(this);
}

void MBanner::setTitle(const QString &title)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_NotificationAreaSink::titles.replace(index, title);
    } else {
        Ut_NotificationAreaSink::titles.append(title);
    }
}

void MBanner::setSubtitle(const QString &subtitle)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_NotificationAreaSink::subtitles.replace(index, subtitle);
    } else {
        Ut_NotificationAreaSink::subtitles.append(subtitle);
    }
}

void MBanner::setIconID(const QString &iconId)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_NotificationAreaSink::buttonIcons.replace(index, iconId);
    } else {
        Ut_NotificationAreaSink::buttonIcons.append(iconId);
    }
}

// MSceneWindow stubs (used by NotificationAreaSink)
void MSceneWindow::disappear()
{
    emit disappeared();
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    return true;
}

// QDir Stubs
bool QDir::exists() const
{
    return true;
}

QStringList QDir::entryList(const QStringList &, Filters, SortFlags) const
{
    return QStringList();
}

QList<QString> Ut_NotificationAreaSink::titles;
QList<QString> Ut_NotificationAreaSink::subtitles;
QList<QString> Ut_NotificationAreaSink::buttonIcons;
QList<QString> Ut_NotificationAreaSink::contents;
QList<MBanner *> Ut_NotificationAreaSink::notifications;
QList<MBanner *> Ut_NotificationAreaSink::destroyedNotifications;

// Tests
void Ut_NotificationAreaSink::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_notificationareasink";
    app = new MApplication(argc, &app_name);
}

void Ut_NotificationAreaSink::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
    delete settings;
}

void Ut_NotificationAreaSink::init()
{
    sink = new NotificationAreaSink();
    settings = new QSettings();
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(addGroup(uint, const NotificationParameters &)), sink, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(this, SIGNAL(removeGroup(uint)), sink, SLOT(removeGroup(uint)));
    connect(sink, SIGNAL(addNotification(MBanner &)), this, SLOT(addNotification(MBanner &)));
    connect(sink, SIGNAL(removeNotification(MBanner &)), this, SLOT(removeNotification(MBanner &)));
}

void Ut_NotificationAreaSink::addNotification(MBanner &notification)
{
    Ut_NotificationAreaSink::notifications.append(&notification);

    notification.setParentItem(new MWidget());
}

void Ut_NotificationAreaSink::removeNotification(MBanner &notification)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(&notification);

    if (index >= 0) {
        Ut_NotificationAreaSink::titles.removeAt(index);
        Ut_NotificationAreaSink::subtitles.removeAt(index);
        Ut_NotificationAreaSink::buttonIcons.removeAt(index);
        Ut_NotificationAreaSink::notifications.removeAt(index);

        notification.setParentItem(0);
    }
}

void Ut_NotificationAreaSink::cleanup()
{
    delete sink;
    titles.clear();
    subtitles.clear();
    buttonIcons.clear();
    contents.clear();
    notifications.clear();
    destroyedNotifications.clear();
}
void Ut_NotificationAreaSink::testAddNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));

    // Create three notifications - two with a content link and one without
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0");
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1");
    emit addNotification(Notification(1, 0, 2, parameters1, Notification::SystemEvent, 1000));
    TestNotificationParameters parameters2("title2", "subtitle2", "buttonicon2", "");
    emit addNotification(Notification(2, 0, 2, parameters2, Notification::ApplicationEvent, 1000));

    // Check that the addNotification() signal was emitted by the sink two times
    QCOMPARE(addSpy.count(), 2);
    QCOMPARE(notifications.count(), 2);
}

void Ut_NotificationAreaSink::testUpdateNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));

    // Add two notifications with the same id; the second should update the existing one.
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0");
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));
    QCOMPARE(addSpy.count(), 1);
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1");
    emit addNotification(Notification(0, 0, 2, parameters1, Notification::ApplicationEvent, 1000));
    QCOMPARE(addSpy.count(), 1);
    QCOMPARE(notifications.count(), 1);

    // TODO: even though contents.length is 2, there's only 1 action in the mnotification
    // clearing of the actions should be stubbed somehow...
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[0], QString("content0"));
    QCOMPARE(contents[1], QString("content1"));
}

void Ut_NotificationAreaSink::testRemoveNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));
    QSignalSpy removeSpy(sink, SIGNAL(removeNotification(MBanner &)));

    // Create three notifications
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0");
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::SystemEvent, 1000));
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1");
    emit addNotification(Notification(1, 0, 2, parameters1, Notification::ApplicationEvent, 1000));
    TestNotificationParameters parameters2("title2", "subtitle2", "buttonicon2", "");
    emit addNotification(Notification(2, 0, 2, parameters2, Notification::ApplicationEvent, 1000));

    // Check that the addNotification() signal was emitted by the sink two times
    QCOMPARE(addSpy.count(), 2);
    QCOMPARE(notifications.count(), 2);

    // Cancel the second one
    emit removeNotification(1);

    // Check that the removeNotification() signal was emitted by the sink once
    QCOMPARE(removeSpy.count(), 1);
    QCOMPARE(notifications.count(), 1);
    QCOMPARE(destroyedNotifications.count(), 1);

    // Recreate the second notification and create an additional one
    emit addNotification(Notification(1, 0, 2, parameters1, Notification::ApplicationEvent, 1000));
    TestNotificationParameters parameters3("title3", "subtitle3", "buttonicon3", "");
    emit addNotification(Notification(3, 0, 2, parameters3, Notification::SystemEvent, 1000));

    // Check that the addNotification() signal was emitted by the sink three times in total
    QCOMPARE(addSpy.count(), 3);
    QCOMPARE(buttonIcons.length(), 2);
    QCOMPARE(buttonIcons[0], QString("buttonicon2"));
    QCOMPARE(buttonIcons[1], QString("buttonicon1"));
}

void Ut_NotificationAreaSink::testAddGroup()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));

    // Creating a group should not send signals, just create the mnotification
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0");
    emit addGroup(1, parameters0);
    QCOMPARE(addSpy.count(), 0);
    QCOMPARE(notifications.count(), 0);
}

void Ut_NotificationAreaSink::testRemoveGroup()
{
    QSignalSpy remSpy(sink, SIGNAL(removeNotification(MBanner &)));

    // A signal from removeGroup is not sent if no notification has been added to the group
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0");
    emit addGroup(1, parameters0);
    emit removeGroup(1);
    QCOMPARE(remSpy.count(), 0);

    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    emit removeGroup(1);
    QCOMPARE(remSpy.count(), 1);
}

void Ut_NotificationAreaSink::testRemovingNotificationsWhenNoNotificationLeftGroupBannerIsRemoved()
{
    QSignalSpy remSpy(sink, SIGNAL(removeNotification(MBanner &)));

    TestNotificationParameters parameters0("title0", "subtitle0", "icon0", "content0");
    emit addGroup(1, parameters0);
    // Add two notifications to this group
    TestNotificationParameters parameters1("title1", "subtitle1", "icon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(1, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    // Remove first notification
    emit(removeNotification(0));
    // There is still one notification in the group so banner is not removed.
    QCOMPARE(remSpy.count(), 0);
    // Last notification in group removed, so banner will be removed
    emit(removeNotification(1));
    QCOMPARE(remSpy.count(), 1);
}

void Ut_NotificationAreaSink::testAddNotificationToGroup()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));
    TestNotificationParameters parameters0("title0", "subtitle0", "icon0", "content0");
    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("title1", "subtitle1", "icon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));

    QCOMPARE(addSpy.count(), 1);
    QCOMPARE(notifications.count(), 1);
}

void Ut_NotificationAreaSink::testAddNewNotificationToGroupUpdatesNotificationArea()
{
    QSignalSpy updateSpy(sink, SIGNAL(notificationAddedToGroup(MBanner &)));
    TestNotificationParameters parameters0("title0", "subtitle0", "icon0", "content0");
    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("title1", "subtitle1", "icon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    QCOMPARE(updateSpy.count(), 0);
    emit addNotification(Notification(1, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    QCOMPARE(updateSpy.count(), 1);
}

void Ut_NotificationAreaSink::testUpdateGroup()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0");
    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));

    QCOMPARE(titles.length(), 1);
    QCOMPARE(titles[0], QString("title0"));
    QCOMPARE(subtitles.length(), 1);
    QCOMPARE(subtitles[0], QString("subtitle0"));
    QCOMPARE(buttonIcons.length(), 1);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(contents.length(), 1);
    QCOMPARE(contents[0], QString("content0"));

    emit addGroup(1, parameters1);

    QCOMPARE(titles.length(), 1);
    QCOMPARE(titles[0], QString("title1"));
    QCOMPARE(subtitles.length(), 1);
    QCOMPARE(subtitles[0], QString("subtitle1"));
    QCOMPARE(buttonIcons.length(), 1);
    QCOMPARE(buttonIcons[0], QString("buttonicon1"));
    // TODO: even though contents.length is 2, there's only 1 action in the mnotification
    // clearing of the actions should be stubbed somehow...
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[1], QString("content1"));
}

void Ut_NotificationAreaSink::testApplyPrivacySetting_data()
{
    QTest::addColumn<bool>("honorPrivacy");
    QTest::addColumn<bool>("privacyEnabled");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("subtitle");

    QTest::newRow("Privacy enabled and honored") << true << true << "test0" << QString();
    QTest::newRow("Privacy disabled and honored") << true << false << "title0" << "subtitle0";
    QTest::newRow("Privacy enabled but not honored") << false << true << "title0" << "subtitle0";
    QTest::newRow("Privacy disabled and not honored") << false << false << "title0" << "subtitle0";
}

void Ut_NotificationAreaSink::testApplyPrivacySetting()
{
    QFETCH(bool, honorPrivacy);
    QFETCH(bool, privacyEnabled);
    QFETCH(QString, title);
    QFETCH(QString, subtitle);

    sink->setHonorPrivacySetting(honorPrivacy);
    gMGConfPrivateNotificationValue = privacyEnabled;

    // Create a notification
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0");
    parameters0.add(NotificationWidgetParameterFactory::createGenericTextIdParameter("test0"));
    parameters0.add(NotificationWidgetParameterFactory::createGenericTextCatalogueParameter("test0"));
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));

    // Set the privacy mode
    connect(this, SIGNAL(privacySettingChanged(bool)), sink, SIGNAL(privacySettingChanged(bool)));
    emit privacySettingChanged(privacyEnabled);

    // The titles should be as expected
    QCOMPARE(titles.length(), 1);
    QCOMPARE(titles[0], title);
    QCOMPARE(subtitles.length(), 1);
    QCOMPARE(subtitles[0], subtitle);

    // Update the notification
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));

    // The titles should still be as expected
    QCOMPARE(titles.length(), 1);
    QCOMPARE(titles[0], title);
    QCOMPARE(subtitles.length(), 1);
    QCOMPARE(subtitles[0], subtitle);
}

QTEST_APPLESS_MAIN(Ut_NotificationAreaSink)

