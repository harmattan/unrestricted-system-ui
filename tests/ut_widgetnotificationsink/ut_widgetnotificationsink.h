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

#ifndef UT_WIDGETNOTIFICATIONSINK_H
#define UT_WIDGETNOTIFICATIONSINK_H

#include <QtTest/QtTest>
#include <QObject>
#include "notification.h"

class MApplication;
class TestWidgetNotificationSink;
class QGraphicsWidget;
class QAction;
class TestNotificationParameters;

class Ut_WidgetNotificationSink : public QObject
{
    Q_OBJECT

public:
    static QList<QString> contents;
    static QHash<const QGraphicsWidget *, QList<QAction *> > actions;
    static int actionTriggeredCount;

signals:
    void click();

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    void testUpdateActions();
    void testInfoBannerClicking();
    void testInfoBannerClickingWhenNotUserRemovableInParameters();
    void testInfoBannerCreationWithRemoteAction();
    void testInfoBannerCreationWithoutRemoteAction();
    void testInfoBannerCreationWithNotificationParameters();
    void testInfoBannerCreationWithSystemEvent();
    void testInfoBannerCreationWhenPrivacyIsHonoredButNotEnabled();
    void testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledButNoCatalogIdDefined();
    void testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledButNoCatalogNameDefined();
    void testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledAndCatalogPresent();
    void testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledAndCatalogPresentPlural();
    void testUserRemovablePropertyIsSetWhenBannerIsCreated();
    void testWhenNotificationsCreatedThenImageIsSetCorrectly_data();
    void testWhenNotificationsCreatedThenImageIsSetCorrectly();
    void testPrivacySettingValueEmittedWhenHonoringChanges_data();
    void testPrivacySettingValueEmittedWhenHonoringChanges();
    void testPrivacySettingValueEmittedWhenPrivacySettingChanges();
    void testWhenNotificationsCreatedAreNotClickableWhenClickingThemDoesNotWork();
    void testNotificationShownOnlyIfItContainsText();

private:
    // Helper for the "test clicking when not user removable" cases
    void testInfoBannerClickingWhenNotUserRemovable(TestNotificationParameters &parameters);

    // HomeApplication
    MApplication *app;
    // The object being tested
    TestWidgetNotificationSink *m_subject;
};

#endif
