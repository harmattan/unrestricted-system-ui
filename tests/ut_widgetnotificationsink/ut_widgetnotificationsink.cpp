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

#include <MRemoteAction>
#include "ut_widgetnotificationsink.h"
#include "widgetnotificationsink.h"
#include "testnotificationparameters.h"
#include "genericnotificationparameterfactory.h"
#include <MApplication>

// List of event type files
QStringList eventTypeFilesList;

class TestWidgetNotificationSink : public WidgetNotificationSink
{
public:
    MBanner *createInfoBanner(const Notification &);
    MBanner *createInfoBanner(Notification::NotificationType, uint groupId, const NotificationParameters &);
    void addNotification(const Notification &);
    void removeNotification(uint);
    QString determineIconId(const NotificationParameters &);
    void updateActions(MBanner *infoBanner, const Notification &notification);
};

MBanner *TestWidgetNotificationSink::createInfoBanner(const Notification &n)
{
    return WidgetNotificationSink::createInfoBanner(n);
}

MBanner *TestWidgetNotificationSink::createInfoBanner(Notification::NotificationType type, uint groupId, const NotificationParameters &parameters)
{
    return WidgetNotificationSink::createInfoBanner(type, groupId, parameters);
}

void TestWidgetNotificationSink::addNotification(const Notification &)
{
}

void TestWidgetNotificationSink::removeNotification(uint)
{
}

QString TestWidgetNotificationSink::determineIconId(const NotificationParameters &params)
{
    return WidgetNotificationSink::determineIconId(params);
}

void TestWidgetNotificationSink::updateActions(MBanner *infoBanner, const Notification &notification)
{
    WidgetNotificationSink::updateActions(infoBanner, notification.parameters());
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
    return eventTypeFilesList;
}

// QGraphicsWidget stubs (used by MCompositorNotificationSink)
void QGraphicsWidget::addAction(QAction *action)
{
    Ut_WidgetNotificationSink::actions[this].append(action);

    MRemoteAction *dra = dynamic_cast<MRemoteAction *>(action);
    if (dra != NULL) {
        Ut_WidgetNotificationSink::contents.append(dra->toString());
    }
}

void QGraphicsWidget::removeAction(QAction *action)
{
    Ut_WidgetNotificationSink::actions[this].removeAll(action);

    MRemoteAction *dra = dynamic_cast<MRemoteAction *>(action);
    if (dra != NULL) {
        Ut_WidgetNotificationSink::contents.removeAll(dra->toString());
    }
}

QList<QAction *> QGraphicsWidget::actions() const
{
    return Ut_WidgetNotificationSink::actions[this];
}

void QAction::activate(ActionEvent)
{
    Ut_WidgetNotificationSink::actionTriggeredCount++;
}

//constants for loading icon image from path tests
static const int ICON_DEFAULT_SIZE = 64;
static const int ICON_SMALL_SIZE = 40;
static const int ICON_BIG_SIZE = 100;
static const int ICON_TOO_BIG_SIZE = 101;

QList<QString> Ut_WidgetNotificationSink::contents;
QHash<const QGraphicsWidget *, QList<QAction *> > Ut_WidgetNotificationSink::actions;
int Ut_WidgetNotificationSink::actionTriggeredCount;

void Ut_WidgetNotificationSink::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_widgetnotificationsink";
    app = new MApplication(argc, &app_name);
}

void Ut_WidgetNotificationSink::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_WidgetNotificationSink::init()
{
    eventTypeFilesList.append("message-received.conf");
    eventTypeFilesList.append("voicemail.conf");
    m_subject = new TestWidgetNotificationSink();
    contents.clear();
    actions.clear();
    actionTriggeredCount = 0;
}

void Ut_WidgetNotificationSink::cleanup()
{
    delete m_subject;
    eventTypeFilesList.clear();
}

void Ut_WidgetNotificationSink::testUpdateActions()
{
    // Create notification parameters
    Notification notification;
    TestNotificationParameters parameters;
    parameters.add(NotificationWidgetParameterFactory::createActionParameter("content0 0 0 0"));
    notification.setParameters(parameters);

    // Create an info banner with one action
    MBanner infoBanner;
    QAction *action = new QAction(&infoBanner);
    infoBanner.addAction(action);

    // There shouldn't be any MRemoteActions at this point but the action should be added
    QCOMPARE(contents.count(), 0);
    QCOMPARE(actions.count(), 1);
    QCOMPARE(actions[&infoBanner].at(0), action);

    // Update the actions
    m_subject->updateActions(&infoBanner, notification);

    // There should be a MRemoteAction at this point
    QCOMPARE(contents.count(), 1);
    QCOMPARE(contents[0], QString("content0 0 0 0"));
    QCOMPARE(actions.count(), 1);
    QVERIFY(dynamic_cast<MRemoteAction *>(actions[&infoBanner].at(0)) != NULL);
    QCOMPARE(dynamic_cast<MRemoteAction *>(actions[&infoBanner].at(0))->toString(), QString("content0 0 0 0"));
}

void Ut_WidgetNotificationSink::testInfoBannerClicking()
{
    uint notificationID = 0;

    // Create info banner
    QApplication::processEvents();
    TestNotificationParameters parameters;
    parameters.add(NotificationWidgetParameterFactory::createActionParameter("content0 0 0 0"));
    MBanner *infoBanner = m_subject->createInfoBanner(Notification(notificationID, 0, 1, parameters, Notification::ApplicationEvent, 1000));

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 1);
    MRemoteAction *action = dynamic_cast<MRemoteAction *>(actions[infoBanner].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationRemovalRequested(m_subject, SIGNAL(notificationRemovalRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    QApplication::processEvents();

    // Verify that action was triggered
    QCOMPARE(actionTriggeredCount, 1);

    // Verify that request for notification removal was emitted
    QCOMPARE(notificationRemovalRequested.count(), 1);
    QCOMPARE(notificationRemovalRequested.first().at(0).toUInt(), notificationID);

    // Create a group info banner
    uint groupID = 1;
    TestNotificationParameters groupParameters;
    groupParameters.add(NotificationWidgetParameterFactory::createActionParameter("content1 1 1 1"));
    infoBanner = m_subject->createInfoBanner(Notification::ApplicationEvent, groupID, groupParameters);

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 2);
    action = dynamic_cast<MRemoteAction *>(actions[infoBanner].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationGroupClearingRequested(m_subject, SIGNAL(notificationGroupClearingRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    QApplication::processEvents();

    // Verify that action was triggered
    QCOMPARE(actionTriggeredCount, 2);

    // Verify that request for notification removal was emitted
    QCOMPARE(notificationGroupClearingRequested.count(), 1);
    QCOMPARE(notificationGroupClearingRequested.first().at(0).toUInt(), groupID);
}

void Ut_WidgetNotificationSink::testInfoBannerClickingWhenNotUserRemovable(TestNotificationParameters &parameters)
{
    // Create info banner from the parameters by first adding an action into the parameters
    QApplication::processEvents();
    uint notificationID = 0;
    parameters.add(NotificationWidgetParameterFactory::createActionParameter("content0 0 0 0"));
    MBanner *infoBanner = m_subject->createInfoBanner(Notification(notificationID, 0, 1, parameters, Notification::ApplicationEvent, 1000));

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 1);
    MRemoteAction *action = dynamic_cast<MRemoteAction *>(actions[infoBanner].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationRemovalRequested(m_subject, SIGNAL(notificationRemovalRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    QApplication::processEvents();

    // Verify that action was triggered even though the info banner is not user removable
    QCOMPARE(actionTriggeredCount, 1);

    // Verify that request for notification removal was not emitted since the info banner is not user removable
    QCOMPARE(notificationRemovalRequested.count(), 0);
}

void Ut_WidgetNotificationSink::testInfoBannerClickingWhenNotUserRemovableInParameters()
{
    TestNotificationParameters parameters;
    parameters.add(NotificationWidgetParameterFactory::createUserRemovableParameter(false));

    testInfoBannerClickingWhenNotUserRemovable(parameters);
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithRemoteAction()
{
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");

    MBanner *infoBanner = m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020));
    QCOMPARE(infoBanner->title(), QString("title0"));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle0"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon0"));
    QCOMPARE(infoBanner->actions().count(), 1);
    MRemoteAction *remoteAction = dynamic_cast<MRemoteAction *>(infoBanner->actions().at(0));
    QVERIFY(remoteAction != NULL);
    QCOMPARE(remoteAction->toString(), QString("content0 0 0 0"));
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithoutRemoteAction()
{
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    MBanner *infoBanner = m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020));
    QCOMPARE(infoBanner->title(), QString("title1"));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle1"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
    QCOMPARE(infoBanner->actions().count(), 0);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithSystemEvent()
{
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    MBanner *infoBanner = m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::SystemEvent, 1020));
    // Check that title is not set for system event
    QCOMPARE(infoBanner->title(), QString(""));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle1"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
    QCOMPARE(infoBanner->actions().count(), 0);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithNotificationParameters()
{
    TestNotificationParameters parameters("title3", "subtitle3", "buttonicon3", "content1 2 3 4");
    MBanner *infoBanner = m_subject->createInfoBanner(Notification::ApplicationEvent, 1, parameters);
    QCOMPARE(infoBanner->title(), QString("title3"));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle3"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon3"));
    QCOMPARE(infoBanner->actions().count(), 1);
    MRemoteAction *remoteAction = dynamic_cast<MRemoteAction *>(infoBanner->actions().at(0));
    QVERIFY(remoteAction != NULL);
    QCOMPARE(remoteAction->toString(), QString("content1 2 3 4"));
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testUserRemovablePropertyIsSetWhenBannerIsCreated()
{
    TestNotificationParameters parameters;

    parameters.add(NotificationWidgetParameterFactory::createUserRemovableParameter(true));
    MBanner* infoBanner = m_subject->createInfoBanner(Notification::ApplicationEvent, 1, parameters);
    QCOMPARE(infoBanner->property(WidgetNotificationSink::USER_REMOVABLE_PROPERTY).toBool(), true);

    parameters.add(NotificationWidgetParameterFactory::createUserRemovableParameter(false));
    infoBanner = m_subject->createInfoBanner(Notification::ApplicationEvent, 1, parameters);
    QCOMPARE(infoBanner->property(WidgetNotificationSink::USER_REMOVABLE_PROPERTY).toBool(), false);
}

QTEST_APPLESS_MAIN(Ut_WidgetNotificationSink)
