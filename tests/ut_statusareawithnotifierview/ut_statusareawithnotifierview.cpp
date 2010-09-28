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

#include "ut_statusareawithnotifierview.h"
#include "statusareawithnotifierview.h"
#include "statusarea.h"
#include "statusareaview.h"
#include "statusareastyle.h"
#include <MApplication>
#include "clock_stub.h"
#include "statusindicator_stub.h"
#include "contextframeworkcontext_stub.h"
#include "alarmstatusindicator_stub.h"
#include "batterystatusindicator_stub.h"
#include "phonenetworksignalstrengthstatusindicator_stub.h"
#include "phonenetworktypestatusindicator_stub.h"
#include "internetconnectionstatusindicator_stub.h"
#include "bluetoothstatusindicator_stub.h"
#include "gpsstatusindicator_stub.h"
#include "presencestatusindicator_stub.h"
#include "statusindicator_stub.h"
#include "phonenetworkstatusindicator_stub.h"
#include "inputmethodstatusindicator_stub.h"
#include "callstatusindicator_stub.h"
#include "profilestatusindicator_stub.h"
#include "notificationarea_stub.h"
#include "notificationstatusindicator_stub.h"
#include "notifiernotificationsink_stub.h"
#include "notificationsink_stub.h"
#include "statusindicatormenuadaptor_stub.h"
#include "statusindicatormenuwindowstyle.h"
#include <MSceneWindow>
#include <QGraphicsLayout>
#include <MWidget>
#include "x11wrapper_stub.h"

class MyStyle : public MStyle
{};

const MStyle *MTheme::style(const char *styleClassName,
                                const QString &objectName)
{
    Q_UNUSED(styleClassName);
    Q_UNUSED(objectName);
    return new MyStyle();
};

bool isNotificationArea = true;
const bool& StatusIndicatorMenuWindowStyle::notificationArea() const
{
    return isNotificationArea;
}

int Ut_StatusAreaWithNotifierView::windowExecutionCount;
int Ut_StatusAreaWithNotifierView::windowRejectionCount;

// SceneWindow stubs
void MSceneWindow::appear(MSceneWindow::DeletionPolicy)
{
    Ut_StatusAreaWithNotifierView::windowExecutionCount++;
}

void MSceneWindow::disappear()
{
    Ut_StatusAreaWithNotifierView::windowRejectionCount++;
}

// Called before the first testfunction is executed
void Ut_StatusAreaWithNotifierView::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusareawithnotifierview";
    app = new MApplication(argc, &app_name);
}

// Called after the last testfunction was executed
void Ut_StatusAreaWithNotifierView::cleanupTestCase()
{
    delete app;
}

// Called before each testfunction is executed
void Ut_StatusAreaWithNotifierView::init()
{
    statusArea = new StatusArea;
    m_subject = new StatusAreaWithNotifierView(statusArea);
    statusArea->setView(m_subject);
    gNotificationStatusIndicatorStub->stubReset();
}

// Called after every testfunction
void Ut_StatusAreaWithNotifierView::cleanup()
{
    delete statusArea;
    statusArea = NULL;
}

void Ut_StatusAreaWithNotifierView::testNotifierAddedToLayout()
{
    QVERIFY(m_subject->landscapeNotificationIndicator->parentWidget() != NULL);
    QVERIFY(m_subject->portraitNotificationIndicator->parentWidget() != NULL);
}

void Ut_StatusAreaWithNotifierView::testStatusIndicatorMenuVisibilityChangedSignal()
{
    connect(this, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), statusArea, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)));

    // verify that we don't do anything when visibility is false
    emit statusIndicatorMenuVisibilityChanged(false);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("clearSink"), 0);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("disableNotificationAdditions"), 1);
    QCOMPARE(gNotifierNotificationSinkStub->stubLastCallTo("disableNotificationAdditions").parameter<bool>(0), false);

    // verify that sink is cleared when status menu comes visible
    emit statusIndicatorMenuVisibilityChanged(true);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("clearSink"), 1);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("disableNotificationAdditions"), 2);
    QCOMPARE(gNotifierNotificationSinkStub->stubLastCallTo("disableNotificationAdditions").parameter<bool>(0), true);
}

QTEST_APPLESS_MAIN(Ut_StatusAreaWithNotifierView)
