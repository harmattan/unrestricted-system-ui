
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

#include "ut_statusindicatormenudropdownview.h"
#include "statusindicatormenudropdownview.h"
#include "statusindicatormenu.h"
#include <MApplication>
#include <MApplicationExtensionArea>
#include <QGraphicsSceneMouseEvent>
#include <QtTest/QtTest>
#include <MOverlay>
#include <MButton>
#include <QGraphicsLinearLayout>
#include "notificationarea_stub.h"
#include <MPannableViewport>
#include "x11wrapper_stub.h"
#include "statusindicatormenuwindowstyle.h"
#include <MSceneManager>

#ifdef HAVE_QMSYSTEM
#include "qmlocks_stub.h"
#endif

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

int gMApplicationExtensionArea_init_callCount = 0;
QRegExp mApplicationExtensionAreaInProcessFilterDuringInit;
QRegExp mApplicationExtensionAreaVerticalFilterDuringInit;
QRegExp mApplicationExtensionAreaOutOfProcessFilterDuringInit;
QStringList mApplicationExtensionAreaOrderDuringInit;
QStringList mApplicationExtensionAreaVerticalOrderDuringInit;
bool MApplicationExtensionArea::init()
{
    ++gMApplicationExtensionArea_init_callCount;

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


bool showNotificationArea = true;

StatusIndicatorMenuWindowStyleContainer::StatusIndicatorMenuWindowStyleContainer() :
  d_ptr(NULL)
{
}


const char* StatusIndicatorMenuWindowStyleContainer::styleType() const
{
    return "MWidgetStyle";
}

StatusIndicatorMenuWindowStyleContainer::~StatusIndicatorMenuWindowStyleContainer()
{
}

const bool& StatusIndicatorMenuWindowStyle::notificationArea() const
{
    return showNotificationArea;
}


void MSceneManager::appearSceneWindowNow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy)
{
    Q_UNUSED(policy);
    sceneWindow->setVisible(true);
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *sceneWindow)
{
    sceneWindow->setVisible(false);
}

QSize MSceneManager::visibleSceneSize() const
{
    return QSize(864, 480);
}


QObject *findChildItemByObjectName(QGraphicsItem* root, const QString &name) {
    QObject* ob = dynamic_cast<QObject*>(root);

    if (ob)
        if (ob->objectName() == name)
            return ob;

    foreach(QGraphicsItem *i, root->childItems()) {
        ob = findChildItemByObjectName(i, name);

        if (ob)
            return ob;
    }

    return NULL;
}


void Ut_StatusIndicatorMenuDropDownView::init()
{
    gX11WrapperStub->stubReset();
    controller = new StatusIndicatorMenu();
    controller->setModel(new MWidgetModel());
    m_subject = new StatusIndicatorMenuDropDownView(controller);
    controller->setView(m_subject);

    controller->setGeometry(QRectF(0, 0, 800, 400));

    gSetVisible.first = 0;
    gSetVisible.second = false;
    gMApplicationExtensionArea_init_callCount = 0;

    connect(this, SIGNAL(positionOrSizeChanged()), m_subject, SLOT(setPannabilityAndLayout()));
}

void Ut_StatusIndicatorMenuDropDownView::cleanup()
{
    delete m_subject;
    showNotificationArea = true;
}

void Ut_StatusIndicatorMenuDropDownView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatormenudropdownview";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicatorMenuDropDownView::cleanupTestCase()
{
    delete app;
}


void Ut_StatusIndicatorMenuDropDownView::testCloseButtonOverlay()
{
    QVERIFY(m_subject->closeButtonOverlay);
    QCOMPARE(m_subject->closeButtonOverlay->objectName(), QString("CloseButtonOverlay"));

    // Check the number of children of overlay and its layout
    QCOMPARE(m_subject->closeButtonOverlay->childItems().count(), 4);
    QCOMPARE(m_subject->closeButtonOverlay->layout()->count(), 3);

    // Check the type of the children of the overlay
    QVERIFY(dynamic_cast<EventEaterWidget *> (m_subject->closeButtonOverlay->layout()->itemAt(0)));
    QVERIFY(dynamic_cast<MButton *> (m_subject->closeButtonOverlay->layout()->itemAt(1)));
    QVERIFY(dynamic_cast<EventEaterWidget *> (m_subject->closeButtonOverlay->layout()->itemAt(2)));

    // Test that status indicator menu close overlay accept events outside close button
    EventEaterWidget *overlayWidget = new EventEaterWidget;
    QGraphicsSceneMouseEvent event;
    overlayWidget->mousePressEvent(&event);
    QCOMPARE(event.isAccepted(), true);
    delete overlayWidget;
}

void Ut_StatusIndicatorMenuDropDownView::testConnections()
{
    QObject *ob = findChildItemByObjectName(controller, "StatusIndicatorMenuTopRowExtensionButton");
    QVERIFY(disconnect(ob, SIGNAL(clicked()), controller, SLOT(launchControlPanelAndHide())));

    ob = findChildItemByObjectName(controller, "StatusIndicatorMenuCloseButton");
    QVERIFY(disconnect(ob, SIGNAL(clicked()), controller, SLOT(hideRequested())));

    ob = findChildItemByObjectName(m_subject->closeButtonOverlay, "StatusIndicatorMenuCloseButton");
    QVERIFY(disconnect(ob, SIGNAL(clicked()), controller, SLOT(hideRequested())));

    QVERIFY(disconnect(controller, SIGNAL(hideRequested()), m_subject, SLOT(resetViewport())));

    QObject *extensionArea = findChildItemByObjectName(controller, "StatusIndicatorMenuTopRowExtensionArea");
    QVERIFY(disconnect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*))));

    extensionArea = findChildItemByObjectName(controller, "StatusIndicatorMenuVerticalExtensionArea");
    QVERIFY(disconnect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*))));
}

void Ut_StatusIndicatorMenuDropDownView::testWhenPressedBelowMenuContentsThenWindowShouldHide()
{
    PannedWidgetController* pannedWidget = dynamic_cast<PannedWidgetController*>(m_subject->pannableViewport->widget());
    QVERIFY(pannedWidget);

    QSizeF preferredSize = pannedWidget->effectiveSizeHint(Qt::PreferredSize);
    // Set the geometry of the widget to something very high so that there is empty space below the real contents that can be pressed
    pannedWidget->setGeometry(QRect(0, 0, preferredSize.width(), preferredSize.height() + 1000));

    QGraphicsSceneMouseEvent event;
    // Set the event position close to the bottom edge, just inside the widget
    event.setPos(QPointF(0, pannedWidget->geometry().height() - 1));

    QSignalSpy spy(controller, SIGNAL(hideRequested()));

    // Check that if the mouse is pressed but not released, then window is not closed
    pannedWidget->mousePressEvent(&event);
    QCOMPARE(spy.count(), 0);

    // Check that window is closed when mouse is released
    pannedWidget->mouseReleaseEvent(&event);
    QCOMPARE(spy.count(), 1);
}

void Ut_StatusIndicatorMenuDropDownView::testCloseButtonPosition()
{
    // When the pannable viewport has not been panned the close button overlay should not be visible
    m_subject->pannableViewport->setPosition(QPointF());
    QCOMPARE(m_subject->closeButtonOverlay->isVisible(), false);

    m_subject->pannableViewport->setPosition(QPointF(0, -m_subject->controller->sceneManager()->visibleSceneSize().height()));
    QCOMPARE(m_subject->closeButtonOverlay->isVisible(), true);
}

void Ut_StatusIndicatorMenuDropDownView::testSetPannability()
{
    PannedWidgetController* pannedWidget = dynamic_cast<PannedWidgetController*>(m_subject->pannableViewport->widget());
    QVERIFY(pannedWidget);

    qreal pannedWidgetContentHeight = pannedWidget->bottommostWidget()->geometry().height();

    // When the pannable viewport is taller than the panned widget there is no need to pan and the pannability should be disabled
    m_subject->pannableViewport->setGeometry(QRectF(0, 0, 1, pannedWidgetContentHeight + 1000));
    emit positionOrSizeChanged();
    QCOMPARE(m_subject->pannableViewport->isEnabled(), false);

    // When the pannable viewport is not taller than the panned widget there is a need to pan and the pannability should be enabled
    m_subject->pannableViewport->setGeometry(QRectF(0, 0, 1, 1));
    emit positionOrSizeChanged();
    QCOMPARE(m_subject->pannableViewport->isEnabled(), true);
}

void Ut_StatusIndicatorMenuDropDownView::testPannableAreaBackgroundWidget()
{
    const QGraphicsWidget *closeButtonRow = static_cast<PannedWidgetController *>(m_subject->pannableViewport->widget())->bottommostWidget();

    // When the pannable viewport has been panned above extension area, the background height should be 0
    m_subject->pannableViewport->setPosition(QPointF(0, m_subject->pannableViewport->geometry().height()));
    emit positionOrSizeChanged();
    QCOMPARE(m_subject->backgroundWidget->minimumHeight(), qreal(0));
    QCOMPARE(m_subject->backgroundWidget->maximumHeight(), qreal(0));

    // When the pannable viewport has not been panned the background height should reach the bottom of the screen
    m_subject->pannableViewport->setPosition(QPointF());
    emit positionOrSizeChanged();
    qreal expectedHeight = (closeButtonRow->mapToItem(controller, QPointF(0, closeButtonRow->geometry().height())).y() - m_subject->pannableViewport->mapToItem(controller, QPointF()).y()) / 2;
    QCOMPARE(m_subject->backgroundWidget->minimumHeight(), expectedHeight);
    QCOMPARE(m_subject->backgroundWidget->maximumHeight(), expectedHeight);

    // When the pannable viewport has been panned below the extension area the background widget should also be there
    m_subject->pannableViewport->setPosition(QPointF(0, -m_subject->pannableViewport->geometry().height()));
    emit positionOrSizeChanged();
    expectedHeight = (closeButtonRow->mapToItem(controller, QPointF(0, closeButtonRow->geometry().height())).y() - m_subject->pannableViewport->mapToItem(controller, QPointF()).y()) / 2;
    QCOMPARE(m_subject->backgroundWidget->minimumHeight(), expectedHeight);
    QCOMPARE(m_subject->backgroundWidget->maximumHeight(), expectedHeight);
}

void Ut_StatusIndicatorMenuDropDownView::testTopRowInitialization()
{
    QCOMPARE(mApplicationExtensionAreaInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
    QCOMPARE(mApplicationExtensionAreaInProcessFilterDuringInit, QRegExp("/statusindicatormenu-(alarms|internetconnection|presence|profile).desktop$"));
    QCOMPARE(mApplicationExtensionAreaOutOfProcessFilterDuringInit, QRegExp("$^"));
    QCOMPARE(mApplicationExtensionAreaOrderDuringInit, ((QStringList() << "statusindicatormenu-alarms.desktop" << "statusindicatormenu-internetconnection.desktop" << "statusindicatormenu-presence.desktop" << "statusindicatormenu-profile.desktop")));
}

void Ut_StatusIndicatorMenuDropDownView::testVerticalExtensionArea()
{
    QCOMPARE(mApplicationExtensionAreaInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
    QCOMPARE(mApplicationExtensionAreaVerticalFilterDuringInit, QRegExp("/statusindicatormenu-(call|transfer).desktop$"));
    QCOMPARE(mApplicationExtensionAreaVerticalOrderDuringInit, ((QStringList() << "statusindicatormenu-call.desktop" << "statusindicatormenu-transfer.desktop")));
}


void Ut_StatusIndicatorMenuDropDownView::testWhenNotificationAreaIsDisabledInStyleThenNotificationAreaIsNotCreated()
{
    cleanup();
    showNotificationArea = false;
    init();
    MWidgetController * contentWidget = dynamic_cast<MWidgetController *>(m_subject->pannableViewport->widget()->layout()->itemAt(0));
    QCOMPARE(contentWidget->layout()->count(), 1);
}

void Ut_StatusIndicatorMenuDropDownView::testWhenNotificationAreaIsEnabledInStyleThenNotificationAreaIsCreated()
{
    cleanup();
    showNotificationArea = true;
    init();
    MWidgetController * contentWidget = dynamic_cast<MWidgetController *>(m_subject->pannableViewport->widget()->layout()->itemAt(0));
    QCOMPARE(contentWidget->layout()->count(), 2);
    NotificationArea* notificationArea = dynamic_cast<NotificationArea*>(contentWidget->layout()->itemAt(1));
    QVERIFY(notificationArea);
}

void Ut_StatusIndicatorMenuDropDownView::testWhenWidgetEntersDisplayThenSettingsExtensionAreaGetsInitialized()
{
    connect(this, SIGNAL(displayEntered()), controller, SIGNAL(displayEntered()));
    emit displayEntered();
    QCOMPARE(gMApplicationExtensionArea_init_callCount, 1);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuDropDownView)
