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
#include "notificationgroup_stub.h"
#include "notificationmanager_stub.h"


static char *gAppName = (char *)"./ut_notificationareasink";

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList(gAppName);
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

    model()->setTitle(title);
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

void MBanner::setBannerTimeStamp(const QDateTime &time)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_NotificationAreaSink::timestamps.replace(index, time);
    } else {
        Ut_NotificationAreaSink::timestamps.append(time);
    }
}

void MBanner::setPrefixTimeStamp(const QString &text)
{
    Ut_NotificationAreaSink::prefixTimeStamps[this] = text;
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
QList<QDateTime> Ut_NotificationAreaSink::timestamps;
QHash<MBanner *, QString> Ut_NotificationAreaSink::prefixTimeStamps;
QList<MBanner *> Ut_NotificationAreaSink::notifications;
QList<MBanner *> Ut_NotificationAreaSink::destroyedNotifications;

void MBannerCatcher::mBannerEmitted(MBanner &banner)
{
    banners.append(&banner);
}

// Tests
void Ut_NotificationAreaSink::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    app = new MApplication(argc, &gAppName);
}

void Ut_NotificationAreaSink::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_NotificationAreaSink::init()
{
    sink = new NotificationAreaSink();
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
        Ut_NotificationAreaSink::timestamps.removeAt(index);
        Ut_NotificationAreaSink::prefixTimeStamps.remove(&notification);

        notification.setParentItem(0);
    }
}

void Ut_NotificationAreaSink::cleanup()
{
    delete sink;
    titles.clear();
    subtitles.clear();
    buttonIcons.clear();
    timestamps.clear();
    contents.clear();
    notifications.clear();
    prefixTimeStamps.clear();
    destroyedNotifications.clear();
}
void Ut_NotificationAreaSink::testAddNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));

    // Create three notifications - two with a content link and one without
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0", 123);
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));
    QCOMPARE(timestamps[0].toTime_t(), (uint)123);
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1");
    emit addNotification(Notification(1, 0, 2, parameters1, Notification::SystemEvent, 1000));
    TestNotificationParameters parameters2("title2", "subtitle2", "buttonicon2", "", 12345);
    emit addNotification(Notification(2, 0, 2, parameters2, Notification::ApplicationEvent, 1000));
    QCOMPARE(timestamps[1].toTime_t(), (uint)12345);

    // Check that the addNotification() signal was emitted by the sink two times
    QCOMPARE(addSpy.count(), 2);
    QCOMPARE(notifications.count(), 2);
}

void Ut_NotificationAreaSink::testUpdateNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));

    // Add two notifications with the same id; the second should update the existing one.
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0",123);
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));
    QCOMPARE(addSpy.count(), 1);
    QCOMPARE(timestamps[0].toTime_t(), (uint)123);
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1", 12345);
    emit addNotification(Notification(0, 0, 2, parameters1, Notification::ApplicationEvent, 1000));
    QCOMPARE(addSpy.count(), 1);
    QCOMPARE(timestamps[0].toTime_t(), (uint)12345);
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
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0", 123);
    emit addGroup(1, parameters0);
    QCOMPARE(addSpy.count(), 0);
    QCOMPARE(timestamps[0].toTime_t(), (uint)123);
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
    TestNotificationParameters parameters0("title0", "subtitle0", "icon0", "content0", 123);
    emit addGroup(1, parameters0);
    QCOMPARE(timestamps[0].toTime_t(), (uint)123);
    TestNotificationParameters parameters1("title1", "subtitle1", "icon1", "content1", 12345);
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));

    QCOMPARE(timestamps[1].toTime_t(), (uint)12345);
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

void Ut_NotificationAreaSink::testWhenAddingNewNotificationToGroupThatHasBeenPreviouslyClearedThenGroupBannerIsConstructedCorrectly()
{
    const uint GROUP_ID = 1;
    const uint NOTIFICATION_ID = 1;
    const QString GROUP_SUMMARY("groupSummary");
    const QString GROUP_BODY("groupBody");
    const QString GROUP_ICON("groupIcon");
    const QString GROUP_ACTION("groupAction");
    const QString NOTIFICATION_SUMMARY("notificationSummary");
    const QString NOTIFICATION_BODY("notificationBody");
    const QString NOTIFICATION_ICON("notificationIcon");
    const QString NOTIFICATION_ACTION("notificationAction");

    TestNotificationParameters groupParameters(GROUP_SUMMARY, GROUP_BODY, GROUP_ICON, GROUP_ACTION);
    emit addGroup(GROUP_ID, groupParameters);
    TestNotificationParameters notificationParameters(NOTIFICATION_SUMMARY, NOTIFICATION_BODY, NOTIFICATION_ICON, NOTIFICATION_ACTION, 12345);
    emit addNotification(Notification(NOTIFICATION_ID, GROUP_ID, 2, notificationParameters, Notification::ApplicationEvent, 1000));

    emit(removeNotification(NOTIFICATION_ID));

    MBannerCatcher bannerCatcher;
    connect(sink, SIGNAL(addNotification(MBanner&)), &bannerCatcher, SLOT(mBannerEmitted(MBanner&)));

    emit addNotification(Notification(NOTIFICATION_ID, GROUP_ID, 2, notificationParameters, Notification::ApplicationEvent, 1000));

    QCOMPARE(bannerCatcher.banners.count(), 1);
    MBanner *banner = bannerCatcher.banners.at(0);
    // The banner should have the notification group's data
    QCOMPARE(banner->title(), GROUP_BODY);
    // The banner should have the timestamp of the previous notification
    QCOMPARE(timestamps[0].toTime_t(), (uint)12345);
}

void Ut_NotificationAreaSink::testUpdateGroup()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0", 12345);
    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1", 123456);
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));

    // The titles and subtitles get set twice and the second one is the relevant one
    QCOMPARE(titles.length(), 2);
    QCOMPARE(titles[1], QString("subtitle0"));
    QCOMPARE(subtitles.length(), 2);
    QCOMPARE(subtitles[1], QString("title0"));
    QCOMPARE(buttonIcons.length(), 1);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(timestamps[0].toTime_t(), (uint)12345);
    QCOMPARE(contents.length(), 1);
    QCOMPARE(contents[0], QString("content0"));

    emit addGroup(1, parameters1);

    // The stub is now aware of the banner so updates go to the first occurrence of the banner
    QCOMPARE(titles.length(), 2);
    QCOMPARE(titles[0], QString("subtitle1"));
    QCOMPARE(subtitles.length(), 2);
    QCOMPARE(subtitles[0], QString("title1"));
    QCOMPARE(buttonIcons.length(), 1);
    QCOMPARE(buttonIcons[0], QString("buttonicon1"));
    QCOMPARE(timestamps[0].toTime_t(), (uint)123456);
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
    QTest::newRow("Privacy disabled and honored") << true << false << "subtitle0" << "title0";
    QTest::newRow("Privacy enabled but not honored") << false << true << "subtitle0" << "title0";
    QTest::newRow("Privacy disabled and not honored") << false << false << "subtitle0" << "title0";
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
    QCOMPARE(titles.length(), 2);
    QCOMPARE(titles[1], title);
    QCOMPARE(subtitles.length(), 2);
    QCOMPARE(subtitles[1], subtitle);

    // Update the notification
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));

    // The titles should still be as expected. The stub is now aware of the banner so updates go to the first occurrence of the banner.
    QCOMPARE(titles.length(), 2);
    QCOMPARE(titles[0], title);
    QCOMPARE(subtitles.length(), 2);
    QCOMPARE(subtitles[0], subtitle);
}

void Ut_NotificationAreaSink::testNotificationsFetchedFromNotificationManager()
{
    NotificationManager stubManager;
    QList<Notification> notifications;
    Notification n;
    notifications.append(n);
    gNotificationManagerStub->stubSetReturnValue("notifications", notifications);

    QList<NotificationGroup> groups;
    NotificationGroup ng;
    gNotificationManagerStub->stubSetReturnValue("groups", groups);

    QSignalSpy addSpy(sink, SIGNAL(addNotification(MBanner &)));
    sink->updateCurrentNotifications(stubManager);

    QCOMPARE(gNotificationManagerStub->stubCallCount("notifications"), 1);
    QCOMPARE(gNotificationManagerStub->stubCallCount("groups"), 1);

    QCOMPARE(addSpy.count(), 1);
}

void Ut_NotificationAreaSink::testSetPrefixForNotificationGroupBannerWhenThereIsMoreThanOneNotificationInAGroup()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "icon0", "content0", 123);
    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("title1", "subtitle1", "icon1", "content1", 12345);
    TestNotificationParameters parameters2("title1", "subtitle1", "icon1", "content1", 12346);
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(1, 1, 2, parameters2, Notification::ApplicationEvent, 1000));

    QCOMPARE(notifications.count(), 1);
    QCOMPARE(prefixTimeStamps.count(), 1);
    QCOMPARE(prefixTimeStamps.value(notifications.at(0)).isEmpty(), false);

    removeNotification(0);
    QCOMPARE(prefixTimeStamps.value(notifications.at(0)).isEmpty(), true);
}

QTEST_APPLESS_MAIN(Ut_NotificationAreaSink)
