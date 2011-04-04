
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
#include <MPannableViewport>
#include "notificationarea_stub.h"
#include "sysuid_stub.h"
#include "x11wrapper_modified_stub.h"
#include "statusindicatormenustyle.h"
#include <MSceneManager>

#ifdef HAVE_QMSYSTEM
#include "qmlocks_stub.h"
#endif

// MApplicationExtensionArea stubs
struct MApplicationExtensionAreaProperties
{
    QString mInterface;
    QRegExp mInProcessFilter;
    QRegExp mOutOfProcessFilter;
    QStringList mOrder;
    bool mInitCalled;

    MApplicationExtensionAreaProperties() :
            mInitCalled(false)
    {}
};

QHash<MApplicationExtensionArea*, MApplicationExtensionAreaProperties> gMApplicationExtensionAreaProperties;

MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, QGraphicsItem *)
{
    MApplicationExtensionAreaProperties &props = gMApplicationExtensionAreaProperties[this];
    props.mInterface = interface;
}

MApplicationExtensionArea::~MApplicationExtensionArea() { }

void MApplicationExtensionArea::setInProcessFilter(const QRegExp &inProcessFilter)
{
    MApplicationExtensionAreaProperties &props = gMApplicationExtensionAreaProperties[this];
    props.mInProcessFilter = inProcessFilter;
}

void MApplicationExtensionArea::setOutOfProcessFilter(const QRegExp &outOfProcessFilter)
{
    MApplicationExtensionAreaProperties &props = gMApplicationExtensionAreaProperties[this];
    props.mOutOfProcessFilter = outOfProcessFilter;
}

void MApplicationExtensionArea::setOrder(const QStringList &order)
{
    MApplicationExtensionAreaProperties &props = gMApplicationExtensionAreaProperties[this];
    props.mOrder = order;
}

bool MApplicationExtensionArea::init()
{
    MApplicationExtensionAreaProperties &props = gMApplicationExtensionAreaProperties[this];
    props.mInitCalled = true;

    return true;
}

// QWidget stubs
QPair<void*, bool> gSetVisible(0, false);
void MWindow::setVisible(bool visible)
{
    gSetVisible.first = this;
    gSetVisible.second = visible;
}

// MWidget stub
bool mWidgetSceneManagerExist = false;
MSceneManager *MWidget::sceneManager() const
{
    if (mWidgetSceneManagerExist) {
        return new MSceneManager(NULL, NULL);
    } else {
        return NULL;
    }
}

bool showNotificationArea = true;

StatusIndicatorMenuStyleContainer::StatusIndicatorMenuStyleContainer() :
  d_ptr(NULL)
{
}


const char* StatusIndicatorMenuStyleContainer::styleType() const
{
    return "MWidgetStyle";
}

StatusIndicatorMenuStyleContainer::~StatusIndicatorMenuStyleContainer()
{
}

const bool& StatusIndicatorMenuStyle::notificationArea() const
{
    return showNotificationArea;
}

QList<MSceneWindow*> g_visibleSceneWindows;
bool mSceneManagerDisappearSceneWindowNowCalled = false;
bool mSceneManagerAppearSceneWindowNowCalled = false;

void MSceneManager::appearSceneWindowNow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy)
{
    Q_UNUSED(policy);
    sceneWindow->setVisible(true);
    g_visibleSceneWindows.append(sceneWindow);
    mSceneManagerAppearSceneWindowNowCalled = true;
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *sceneWindow)
{
    sceneWindow->setVisible(false);
    g_visibleSceneWindows.removeAll(sceneWindow);
    mSceneManagerDisappearSceneWindowNowCalled = true;
}

void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    sceneWindow->setVisible(false);
    g_visibleSceneWindows.removeAll(sceneWindow);
}

MSceneWindow::~MSceneWindow()
{
    g_visibleSceneWindows.removeAll(this);
}


QSize mSceneManagerVisibleSceneSize;
QSize MSceneManager::visibleSceneSize() const
{
    return mSceneManagerVisibleSceneSize;
}

void MSceneWindowView::applyStyle()
{
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

struct QTimer_singleShot_params
{
    int msec;
    QObject *receiver;
    QString member;
};
QHash<QObject*, QTimer_singleShot_params> gQTimer_singleShot_params;
void QTimer::singleShot(int msec, QObject *receiver, const char * member)
{
    QTimer_singleShot_params params;
    params.msec = msec;
    params.receiver = receiver;
    params.member = member;
    gQTimer_singleShot_params[receiver] = params;
}


void Ut_StatusIndicatorMenuDropDownView::init()
{
    mSceneManagerVisibleSceneSize = QSize(864, 480);
    mWidgetSceneManagerExist = true;
    gX11WrapperStub->stubReset();
    gQTimer_singleShot_params.clear();
    gMApplicationExtensionAreaProperties.clear();

    controller = new StatusIndicatorMenu();
    controller->setModel(new MWidgetModel());
    m_subject = new StatusIndicatorMenuDropDownView(controller);
    controller->setView(m_subject);

    controller->setGeometry(QRectF(0, 0, 800, 400));

    gSetVisible.first = 0;
    gSetVisible.second = false;

    connect(this, SIGNAL(positionOrSizeChanged()), m_subject, SLOT(setPannabilityAndLayout()));

    mSceneManagerDisappearSceneWindowNowCalled = false;
    mSceneManagerAppearSceneWindowNowCalled = false;
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
    // When the widget fits on the screen the close button overlay should not be visible
    QCOMPARE(m_subject->closeButtonOverlay->isVisible(), false);

    // When the widget is below the screen the close button overlay should be visible
    m_subject->pannableViewport->widget()->setGeometry(m_subject->pannableViewport->widget()->geometry().translated(0, m_subject->controller->sceneManager()->visibleSceneSize().height()));
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
    QCOMPARE(m_subject->pannableViewport->verticalPanningPolicy(), MPannableWidget::PanningAlwaysOff);
    
    // When the pannable viewport is not taller than the panned widget there is a need to pan and the pannability should be enabled
    m_subject->pannableViewport->setGeometry(QRectF(0, 0, 1, 1));
    emit positionOrSizeChanged();
    QCOMPARE(m_subject->pannableViewport->verticalPanningPolicy(), MPannableWidget::PanningAsNeeded);
}

void Ut_StatusIndicatorMenuDropDownView::testPannableAreaBackgroundWidget()
{
    const QGraphicsWidget *closeButtonRow = static_cast<PannedWidgetController *>(m_subject->pannableViewport->widget())->bottommostWidget();

    // When the pannable viewport has been panned above extension area, the background height should be 0
    QRectF pannedWidgetGeometry = m_subject->pannableViewport->widget()->geometry();
    m_subject->pannableViewport->widget()->setGeometry(pannedWidgetGeometry.translated(0, -m_subject->controller->sceneManager()->visibleSceneSize().height()));
    QCOMPARE(m_subject->backgroundWidget->minimumHeight(), qreal(0));
    QCOMPARE(m_subject->backgroundWidget->maximumHeight(), qreal(0));

    // When the pannable viewport has not been panned the background height should reach the bottom of the screen
    m_subject->pannableViewport->widget()->setGeometry(pannedWidgetGeometry);
    qreal expectedHeight = (closeButtonRow->mapToItem(controller, QPointF(0, closeButtonRow->geometry().height())).y() - m_subject->pannableViewport->mapToItem(controller, QPointF()).y()) / 2;
    QCOMPARE(m_subject->backgroundWidget->minimumHeight(), expectedHeight);
    QCOMPARE(m_subject->backgroundWidget->maximumHeight(), expectedHeight);

    // When the pannable viewport has been panned below the extension area the background widget should also be there
    m_subject->pannableViewport->widget()->setGeometry(pannedWidgetGeometry.translated(0, m_subject->controller->sceneManager()->visibleSceneSize().height()));
    expectedHeight = (closeButtonRow->mapToItem(controller, QPointF(0, closeButtonRow->geometry().height())).y() - m_subject->pannableViewport->mapToItem(controller, QPointF()).y()) / 2;
    QCOMPARE(m_subject->backgroundWidget->minimumHeight(), expectedHeight);
    QCOMPARE(m_subject->backgroundWidget->maximumHeight(), expectedHeight);
}

void Ut_StatusIndicatorMenuDropDownView::testTopRowInitialization()
{
    foreach (const MApplicationExtensionAreaProperties &props, gMApplicationExtensionAreaProperties) {
        if (props.mInProcessFilter == QRegExp("/statusindicatormenu-(volume|alarms|internetconnection|presence|profile).desktop$")) {
            QCOMPARE(props.mInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
            QCOMPARE(props.mOutOfProcessFilter, QRegExp("$^"));
            QCOMPARE(props.mOrder, ((QStringList() << "statusindicatormenu-volume.desktop" << "statusindicatormenu-alarms.desktop" << "statusindicatormenu-internetconnection.desktop" << "statusindicatormenu-presence.desktop" << "statusindicatormenu-profile.desktop")));
            return;
        }
    }

    QVERIFY2(false, "No valid extension area was created");
}

void Ut_StatusIndicatorMenuDropDownView::testVerticalExtensionArea()
{
    foreach (const MApplicationExtensionAreaProperties &props, gMApplicationExtensionAreaProperties) {
        if (props.mInProcessFilter == QRegExp("/statusindicatormenu-(call|transfer).desktop$")) {
            QCOMPARE(props.mInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
            QCOMPARE(props.mOrder, ((QStringList() << "statusindicatormenu-call.desktop" << "statusindicatormenu-transfer.desktop")));
            return;
        }
    }

    QVERIFY2(false, "No valid extension area was created");
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

void Ut_StatusIndicatorMenuDropDownView::testWhenWidgetEntersDisplayThenExtensionAreasGetInitialized()
{
    connect(this, SIGNAL(displayEntered()), controller, SIGNAL(displayEntered()));
    emit displayEntered();

    QCOMPARE(gMApplicationExtensionAreaProperties.count(), 2);
    QCOMPARE(gMApplicationExtensionAreaProperties.values().at(0).mInitCalled, true);
    QCOMPARE(gMApplicationExtensionAreaProperties.values().at(1).mInitCalled, true);
}

void Ut_StatusIndicatorMenuDropDownView::testWhenViewIsConstructedThenTimerIsStartedForEnsuringViewability()
{
    QVERIFY(gQTimer_singleShot_params.contains(m_subject));
    const QTimer_singleShot_params &params(gQTimer_singleShot_params.value(m_subject));
    QCOMPARE(params.msec, StatusIndicatorMenuDropDownView::VIEW_INITIALIZATION_DELAY);
    QCOMPARE(params.receiver, m_subject);
    QString timedSlotName(params.member);
    QVERIFY2(timedSlotName.endsWith(QString("ensureIsViewable()")), qPrintable(QString("Actual timed slot name was: ") + timedSlotName));
}

void Ut_StatusIndicatorMenuDropDownView::testCreatedItemsAreRemovedFromTheControllerAndTheScene()
{
    QList<QGraphicsItem*> itemsFromTestSubject = controller->childItems();

    controller->setView(NULL);
    m_subject = NULL;

    // All the items added by the view should have disappeared
    foreach (QGraphicsItem* item, itemsFromTestSubject) {
        QCOMPARE(controller->childItems().contains(item), QBool(false));
    }

    // All the SceneWindows should be gone as well
    QCOMPARE(g_visibleSceneWindows.count(), 0);
}

void Ut_StatusIndicatorMenuDropDownView::testThatCloseButtonOverlayIsAddedToSceneManagerOnlyIfSceneManagerDoesExist()
{
    const QGraphicsWidget *closeButtonRow = static_cast<PannedWidgetController *>(m_subject->pannableViewport->widget())->bottommostWidget();
    // Set closeButtonRow y position to be greater than screen height. This will make closeButtonOverlay to appear.
    mSceneManagerVisibleSceneSize = QSize(864, closeButtonRow->mapToItem(m_subject->controller, QPointF(0, (closeButtonRow->geometry().height() - 1))).y());

    mWidgetSceneManagerExist = false;
    emit positionOrSizeChanged();

    QCOMPARE(mSceneManagerDisappearSceneWindowNowCalled, false);
    QCOMPARE(mSceneManagerAppearSceneWindowNowCalled, false);
    QCOMPARE(m_subject->closeButtonOverlay->isVisible(), false);
    QVERIFY(!g_visibleSceneWindows.contains(m_subject->closeButtonOverlay));

    mWidgetSceneManagerExist = true;
    emit positionOrSizeChanged();

    QCOMPARE(mSceneManagerDisappearSceneWindowNowCalled, false);
    QCOMPARE(mSceneManagerAppearSceneWindowNowCalled, true);
    QCOMPARE(m_subject->closeButtonOverlay->isVisible(), true);
    QVERIFY(g_visibleSceneWindows.contains(m_subject->closeButtonOverlay));
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuDropDownView)
