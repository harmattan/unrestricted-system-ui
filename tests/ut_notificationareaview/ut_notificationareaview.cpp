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
#include <QGraphicsLinearLayout>
#include <QAction>
#include <QSharedPointer>
#include <MApplication>
#include <MButton>
#include "ut_notificationareaview.h"
#include "notificationarea_stub.h"
#include "widgetnotificationsink_stub.h"
#include "notificationareaview.h"

QList<MBanner *> bannersClicked;
void MBanner::click()
{
    bannersClicked.append(this);
}

// Tests
void Ut_NotificationAreaView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_notificationareaview";
    app = new MApplication(argc, &app_name);
}

void Ut_NotificationAreaView::cleanupTestCase()
{
    delete app;
}

void Ut_NotificationAreaView::init()
{
    notificationArea = new NotificationArea;
    notificationArea->setModel(new NotificationAreaModel);
    m_subject = new NotificationAreaView(notificationArea);
    notificationArea->setView(m_subject);
    bannersClicked.clear();
}

void Ut_NotificationAreaView::cleanup()
{
    delete m_subject;
}

MBanner *createBanner(bool removable)
{
    MBanner *banner = new MBanner;
    banner->setProperty(WidgetNotificationSink::USER_REMOVABLE_PROPERTY, removable);
    return banner;
}

void Ut_NotificationAreaView::testClearButtonSignals()
{
    QVERIFY(disconnect(m_subject->clearButton, SIGNAL(clicked()), notificationArea, SLOT(removeAllRemovableBanners())));
}

void Ut_NotificationAreaView::testAddClearButton_data()
{
    QTest::addColumn<int>("numRemovableBanners");
    QTest::addColumn<int>("numNonRemovableBanners");
    QTest::addColumn<bool>("clearButtonExists");

    QTest::newRow("No banners") << 0 << 0 << false;
    QTest::newRow("Only non-removable banners") << 0 << 5 << false;
    QTest::newRow("Only removable banners") << 5 << 0 << true;
    QTest::newRow("Both removable and non-removable banners") << 5 << 5 << true;
}

void Ut_NotificationAreaView::testAddClearButton()
{
    QFETCH(int, numRemovableBanners);
    QFETCH(int, numNonRemovableBanners);
    QFETCH(bool, clearButtonExists);

    BannerList banners;
    for (int i = 0; i < numRemovableBanners; i++) {
        banners << createBanner(true);
    }
    for (int i = 0; i < numNonRemovableBanners; i++) {
        banners << createBanner(false);
    }
    notificationArea->model()->setBanners(banners);

    QCOMPARE(m_subject->bannerLayout->count(), numRemovableBanners + numNonRemovableBanners);
    QCOMPARE(m_subject->clearButton->objectName(), clearButtonExists ? QString("NotificationAreaClearButtonVisible") : QString("NotificationAreaClearButton"));
}

void Ut_NotificationAreaView::testRemoveClearButton()
{
    QSharedPointer<MBanner> banner(createBanner(true));
    BannerList banners;
    banners << banner.data();
    notificationArea->model()->setBanners(banners);
    banners.clear();
    notificationArea->model()->setBanners(banners);
    QCOMPARE(m_subject->clearButton->objectName(), QString("NotificationAreaClearButton"));
}

QTEST_APPLESS_MAIN(Ut_NotificationAreaView)
