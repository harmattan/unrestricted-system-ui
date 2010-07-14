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

#include "ut_statusindicatormenuwindow.h"
#include "statusindicatormenuwindow.h"
#include <MApplication>
#include <MApplicationIfProxy>
#include <MApplicationExtensionArea>
#include <MSceneManager>
#include <QtTest/QtTest>
#include <MOverlay>
#include <MButton>
#include <QGraphicsLinearLayout>
#include "notificationarea_stub.h"
#include <MPannableViewport>
#include "x11wrapper_stub.h"

// MApplicationIfProxy stubs (used by StatusIndicatorMenuWindow)
bool mApplicationIfProxyLaunchCalled = false;
QDBusPendingReply<> MApplicationIfProxy::launch()
{
    mApplicationIfProxyLaunchCalled = true;

    return QDBusPendingReply<>();
}

// MApplicationExtensionArea stubs
QString mApplicationExtensionAreaInterface;
MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, QGraphicsItem *)
{
    mApplicationExtensionAreaInterface = interface;
}

MApplicationExtensionArea::~MApplicationExtensionArea() { }

QRegExp mApplicationExtensionAreaInProcessFilter;
QRegExp mApplicationExtensionAreaVerticalFilter;
void MApplicationExtensionArea::setInProcessFilter(const QRegExp &inProcessFilter)
{
    if (inProcessFilter == QRegExp("/statusindicatormenu-(call|transfer).desktop$")) {
        mApplicationExtensionAreaVerticalFilter = inProcessFilter;
    } else {
        mApplicationExtensionAreaInProcessFilter = inProcessFilter;
    }
}

QRegExp mApplicationExtensionAreaOutOfProcessFilter;
void MApplicationExtensionArea::setOutOfProcessFilter(const QRegExp &outOfProcessFilter)
{
    mApplicationExtensionAreaOutOfProcessFilter = outOfProcessFilter;
}

QStringList mApplicationExtensionAreaOrder;
QStringList mApplicationExtensionAreaVerticalOrder;
void MApplicationExtensionArea::setOrder(const QStringList &order)
{
    if (order.contains("statusindicatormenu-call.desktop") || order.contains("statusindicatormenu-transfer.desktop")) {
        mApplicationExtensionAreaVerticalOrder = order;
    } else {
        mApplicationExtensionAreaOrder = order;
    }
}

QRegExp mApplicationExtensionAreaInProcessFilterDuringInit;
QRegExp mApplicationExtensionAreaVerticalFilterDuringInit;
QRegExp mApplicationExtensionAreaOutOfProcessFilterDuringInit;
QStringList mApplicationExtensionAreaOrderDuringInit;
QStringList mApplicationExtensionAreaVerticalOrderDuringInit;
bool MApplicationExtensionArea::init()
{
    mApplicationExtensionAreaInProcessFilterDuringInit = mApplicationExtensionAreaInProcessFilter;
    mApplicationExtensionAreaVerticalFilterDuringInit = mApplicationExtensionAreaVerticalFilter;
    mApplicationExtensionAreaOutOfProcessFilterDuringInit = mApplicationExtensionAreaOutOfProcessFilter;
    mApplicationExtensionAreaOrderDuringInit = mApplicationExtensionAreaOrder;
    mApplicationExtensionAreaVerticalOrderDuringInit = mApplicationExtensionAreaVerticalOrder;
    return true;
}

// QWidget stubs
QPair<void*, bool> gSetVisible(0, false);
void MWindow::setVisible(bool visible)
{
    gSetVisible.first = this;
    gSetVisible.second = visible;
}

void Ut_StatusIndicatorMenuWindow::init()
{
    gX11WrapperStub->stubReset();
    statusIndicatorMenuWindow = new StatusIndicatorMenuWindow;
    gSetVisible.first = 0;
    gSetVisible.second = false;
    mApplicationIfProxyLaunchCalled = false;
    connect(this, SIGNAL(settingsButtonClicked()), statusIndicatorMenuWindow, SLOT(launchControlPanelAndHide()));
    connect(this, SIGNAL(positionOrSizeChanged()), statusIndicatorMenuWindow, SLOT(setPannabilityAndLayout()));
}

void Ut_StatusIndicatorMenuWindow::cleanup()
{
    delete statusIndicatorMenuWindow;
}

void Ut_StatusIndicatorMenuWindow::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatormenuwindow";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicatorMenuWindow::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorMenuWindow::testMakeVisible()
{
    statusIndicatorMenuWindow->hide();
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
    statusIndicatorMenuWindow->makeVisible();
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && gSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testCloseButtonOverlay()
{
    QVERIFY(statusIndicatorMenuWindow->closeButtonOverlay);
    QCOMPARE(statusIndicatorMenuWindow->closeButtonOverlay->objectName(), QString("CloseButtonOverlay"));

    // Check the number of children of overlay and its layout
    QCOMPARE(statusIndicatorMenuWindow->closeButtonOverlay->childItems().count(), 4);
    QCOMPARE(statusIndicatorMenuWindow->closeButtonOverlay->layout()->count(), 3);

    // Check the type of the children of the overlay
    QVERIFY(dynamic_cast<EventEaterWidget *> (statusIndicatorMenuWindow->closeButtonOverlay->layout()->itemAt(0)));
    QVERIFY(dynamic_cast<MButton *> (statusIndicatorMenuWindow->closeButtonOverlay->layout()->itemAt(1)));
    QVERIFY(dynamic_cast<EventEaterWidget *> (statusIndicatorMenuWindow->closeButtonOverlay->layout()->itemAt(2)));

    // Test status indicator menu window visibility
    QVERIFY(gSetVisible.first != statusIndicatorMenuWindow && !gSetVisible.second);
    connect(this, SIGNAL(clicked()), statusIndicatorMenuWindow, SLOT(hide()));
    emit clicked();
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);

    // Test that status indicator menu close overlay accept events outside close button
    EventEaterWidget *overlayWidget = new EventEaterWidget;
    QGraphicsSceneMouseEvent event;
    overlayWidget->mousePressEvent(&event);
    QCOMPARE(event.isAccepted(), true);
    delete overlayWidget;
}

void Ut_StatusIndicatorMenuWindow::testSettingsButtonClicked()
{
    emit settingsButtonClicked();

    QVERIFY(mApplicationIfProxyLaunchCalled);
}

void Ut_StatusIndicatorMenuWindow::testWhenPressedBelowMenuContentsThenWindowShouldHide()
{
    PannedWidgetController* pannedWidget = dynamic_cast<PannedWidgetController*>(statusIndicatorMenuWindow->pannableViewport->widget());
    QVERIFY(pannedWidget);

    QSizeF preferredSize = pannedWidget->effectiveSizeHint(Qt::PreferredSize);
    // Set the geometry of the widget to something very high so that there is empty space below the real contents that can be pressed
    pannedWidget->setGeometry(QRect(0, 0, preferredSize.width(), preferredSize.height() + 1000));

    QGraphicsSceneMouseEvent event;
    // Set the event position close to the bottom edge, just inside the widget
    event.setPos(QPointF(0, pannedWidget->geometry().height() - 1));
    pannedWidget->mousePressEvent(&event);

    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testCloseButtonPosition()
{
    // When the pannable viewport has not been panned the close button overlay should not be visible
    statusIndicatorMenuWindow->pannableViewport->widget()->setPos(0, 0);
    QCOMPARE(statusIndicatorMenuWindow->closeButtonOverlay->isVisible(), false);

    statusIndicatorMenuWindow->pannableViewport->widget()->setPos(0, statusIndicatorMenuWindow->sceneManager()->visibleSceneSize().height());
    QCOMPARE(statusIndicatorMenuWindow->closeButtonOverlay->isVisible(), true);
}

void Ut_StatusIndicatorMenuWindow::testSetPannability()
{
    PannedWidgetController* pannedWidget = dynamic_cast<PannedWidgetController*>(statusIndicatorMenuWindow->pannableViewport->widget());
    QVERIFY(pannedWidget);

    qreal pannedWidgetContentHeight = pannedWidget->bottommostWidget()->geometry().height();

    // When the pannable viewport is taller than the panned widget there is no need to pan and the pannability should be disabled
    statusIndicatorMenuWindow->pannableViewport->setGeometry(QRectF(0, 0, 1, pannedWidgetContentHeight + 1000));
    emit positionOrSizeChanged();
    QCOMPARE(statusIndicatorMenuWindow->pannableViewport->isEnabled(), false);

    // When the pannable viewport is not taller than the panned widget there is a need to pan and the pannability should be enabled
    statusIndicatorMenuWindow->pannableViewport->setGeometry(QRectF(0, 0, 1, 1));
    emit positionOrSizeChanged();
    QCOMPARE(statusIndicatorMenuWindow->pannableViewport->isEnabled(), true);
}

void Ut_StatusIndicatorMenuWindow::testWindowType()
{
    QVERIFY(statusIndicatorMenuWindow->testAttribute(Qt::WA_X11NetWmWindowTypeMenu));
}

void Ut_StatusIndicatorMenuWindow::testPannableAreaBackgroundWidget()
{
    // When the pannable viewport has been panned above extension area, the background height should be 0
    statusIndicatorMenuWindow->pannableViewport->widget()->setPos(0, -10);
    emit positionOrSizeChanged();
    QCOMPARE(statusIndicatorMenuWindow->backgroundWidget->minimumHeight(), qreal(0));
    QCOMPARE(statusIndicatorMenuWindow->backgroundWidget->maximumHeight(), qreal(0));

    // When the pannable viewport has not been panned the background widget is created and the background height should reach the bottom of the screen
    statusIndicatorMenuWindow->pannableViewport->widget()->setPos(0, 0);
    emit positionOrSizeChanged();
    qreal expectedHeight = statusIndicatorMenuWindow->sceneManager()->visibleSceneSize().height() - statusIndicatorMenuWindow->pannableViewport->mapToItem(statusIndicatorMenuWindow->sceneWindow, QPointF()).y();
    QCOMPARE(statusIndicatorMenuWindow->backgroundWidget->minimumHeight(), expectedHeight);
    QCOMPARE(statusIndicatorMenuWindow->backgroundWidget->maximumHeight(), expectedHeight);

    // When the pannable viewport has been panned below the extension area the background widget should also be there
    statusIndicatorMenuWindow->pannableViewport->widget()->setPos(0, 10);
    emit positionOrSizeChanged();
    QCOMPARE(statusIndicatorMenuWindow->backgroundWidget->minimumHeight(), expectedHeight);
    QCOMPARE(statusIndicatorMenuWindow->backgroundWidget->maximumHeight(), expectedHeight);
}

void Ut_StatusIndicatorMenuWindow::testTopRowInitialization()
{
    QCOMPARE(mApplicationExtensionAreaInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
    QCOMPARE(mApplicationExtensionAreaInProcessFilterDuringInit, QRegExp("/statusindicatormenu-(alarms|internetconnection|presence|profile).desktop$"));
    QCOMPARE(mApplicationExtensionAreaOutOfProcessFilterDuringInit, QRegExp("$^"));
    QCOMPARE(mApplicationExtensionAreaOrderDuringInit, ((QStringList() << "statusindicatormenu-alarms.desktop" << "statusindicatormenu-internetconnection.desktop" << "statusindicatormenu-presence.desktop" << "statusindicatormenu-profile.desktop")));
}

void Ut_StatusIndicatorMenuWindow::testVerticalExtensionArea()
{
    QCOMPARE(mApplicationExtensionAreaInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
    QCOMPARE(mApplicationExtensionAreaVerticalFilterDuringInit, QRegExp("/statusindicatormenu-(call|transfer).desktop$"));
    QCOMPARE(mApplicationExtensionAreaVerticalOrderDuringInit, ((QStringList() << "statusindicatormenu-call.desktop" << "statusindicatormenu-transfer.desktop")));
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuWindow)
