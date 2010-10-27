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
****************************************************************************/

#include <MApplication>
#include <MSceneManager>
#include <MGConfItem>
#include <MImageWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>
#include "ut_lockscreenwithpadlockview.h"
#include "lockscreenwithpadlockview.h"
#include "lockscreen_stub.h"
#include "unlockarea_stub.h"
#include "unlocknotifications_stub.h"
#include <MFeedback>

static QString nameOfLastFeedback;

void MFeedback::play() const
{
    /*
     * Seems that the MWindow sends empty name feedbacks sometimes, we filter
     * those out. I hope these will not cause problems in the future... but we
     * can filter them by name too...
     */
    if(name().isEmpty()) {
        return;
    }

    nameOfLastFeedback = name();
}

QList<MSceneWindow*> gAppearSceneWindowList;
QList<MSceneWindow*> gVisibleSceneWindows;

void MSceneManager::appearSceneWindow(MSceneWindow *window, MSceneWindow::DeletionPolicy /*policy*/)
{
    gAppearSceneWindowList.append(window);
    window->setVisible(true);
    gVisibleSceneWindows.append(window);
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *sceneWindow)
{
    sceneWindow->setVisible(false);
    gVisibleSceneWindows.removeAll(sceneWindow);
}

void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    sceneWindow->setVisible(false);
    gVisibleSceneWindows.removeAll(sceneWindow);
}

MSceneWindow::~MSceneWindow()
{
    gVisibleSceneWindows.removeAll(this);
}

MWidgetView::~MWidgetView()
{
}

void Ut_LockScreenWithPadlockView::init()
{
    controller = new LockScreen(NULL);
    m_subject = new LockScreenWithPadlockView(controller);
    controller->setView(m_subject);
}

void Ut_LockScreenWithPadlockView::cleanup()
{
    gAppearSceneWindowList.clear();
    gUnlockAreaStub->stubReset();
    delete controller;
}

void Ut_LockScreenWithPadlockView::initTestCase()
{
    int   argc = 1;
    static char *app_name = (char *)"./ut_lockscreenwithpadlockview";
    app = new MApplication(argc, &app_name);
}

void Ut_LockScreenWithPadlockView::cleanupTestCase()
{
    delete app;
}

void Ut_LockScreenWithPadlockView::testInitialState()
{
    QVERIFY(!m_subject->dragAndDropOverlay.isVisible());
    QVERIFY(!m_subject->notificationArea->isVisible());
    QVERIFY(disconnect(m_subject->notificationArea, SIGNAL(needToShow(bool)), m_subject, SLOT(showHideNotifications(bool))));
    QVERIFY(disconnect(m_subject, SIGNAL(unlocked()), m_subject->controller, SLOT(sliderUnlocked())));
    QVERIFY(disconnect(m_subject->controller, SIGNAL(resetRequested()), m_subject, SLOT(resetState())));
}

void Ut_LockScreenWithPadlockView::testDraggableIconMovement_data()
{
    QTest::addColumn<int>("layoutDirection");

    QTest::newRow("LeftToRight") << int(Qt::LeftToRight);
    QTest::newRow("RightToLeft") << int(Qt::RightToLeft);
}

void Ut_LockScreenWithPadlockView::checkOverlayPos(const QPointF &eventPos)
{
    QSizeF overlaySize = m_subject->dragAndDropOverlay.preferredSize();
    QPointF pos(eventPos - QPointF(overlaySize.width() / 1.4, overlaySize.height() / 1.4));
    QVERIFY(qFuzzyCompare(m_subject->dragAndDropOverlay.pos().x(), pos.x()));
    QVERIFY(qFuzzyCompare(m_subject->dragAndDropOverlay.pos().y(), pos.y()));
}

void Ut_LockScreenWithPadlockView::testDraggableIconMovement()
{
    QFETCH(int, layoutDirection);

    QSignalSpy unlockSpy(m_subject, SIGNAL(unlocked()));
    m_subject->lockScreenHeader->setLayoutDirection((Qt::LayoutDirection)layoutDirection);
    m_subject->lockScreenHeader->setPreferredSize(100, 10);

    // send three mouse events that ultimately will send the unlocked signal from the window.
    QGraphicsSceneMouseEvent *pressEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    pressEvent->setPos(QPointF(layoutDirection == Qt::LeftToRight ?
                               m_subject->lockScreenHeader->preferredWidth() : 0,
                               m_subject->lockScreenHeader->pos().y()));

    QGraphicsSceneMouseEvent *moveEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent->setPos(QPointF(400, 240));

    QGraphicsSceneMouseEvent *moveEventNotActive = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    moveEventNotActive->setPos(QPointF(400, 0));

    QGraphicsSceneMouseEvent *releaseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);

    // sending a press event the place where it should activate the draggable icon.
    m_subject->mousePressEvent(pressEvent);
    QTest::qWait(50);
    QCoreApplication::processEvents();

    QCOMPARE(m_subject->dragAndDropState, (int)LockScreenWithPadlockView::STATE_MOVING);
    QVERIFY(nameOfLastFeedback == "start-dragndrop");
    checkOverlayPos(pressEvent->pos());
    QVERIFY(m_subject->dragAndDropIcon != 0);
    QCOMPARE(m_subject->dragAndDropOverlay.widget(), static_cast<MWidget*>(m_subject->dragAndDropIcon));
    QCOMPARE(gAppearSceneWindowList.count(), 1);
    QCOMPARE(gAppearSceneWindowList.at(0), &m_subject->dragAndDropOverlay);
    QCOMPARE(m_subject->dragAndDropIcon->objectName(), QString("LockScreenDnDIcon"));
    QVERIFY(m_subject->dragAndDropOverlay.isVisible());
    QCOMPARE(m_subject->lockScreenHeader->objectName(), QString("LockLiftArea"));
    QVERIFY(gUnlockAreaStub->stubLastCallTo("setEnabled").parameter<bool>(0));

    // move the mouse right into the middle of the screen
    m_subject->mouseMoveEvent(moveEvent);
    QCOMPARE(m_subject->dragAndDropState, (int)LockScreenWithPadlockView::STATE_MOVING_ACTIVE);
    QVERIFY(nameOfLastFeedback == "enter-dragndrop-dropzone");
    checkOverlayPos(moveEvent->pos());
    QCOMPARE(m_subject->dragAndDropIcon->objectName(), QString("LockScreenDnDIconActive"));
    QVERIFY(gUnlockAreaStub->stubLastCallTo("setActive").parameter<bool>(0));

    // move back to some non-active place
    m_subject->mouseMoveEvent(moveEventNotActive);
    QCOMPARE(m_subject->dragAndDropState, (int)LockScreenWithPadlockView::STATE_MOVING);
    QVERIFY(nameOfLastFeedback == "exit-dragndrop-dropzone");
    QCOMPARE(m_subject->dragAndDropIcon->objectName(), QString("LockScreenDnDIcon"));
    QVERIFY(!gUnlockAreaStub->stubLastCallTo("setActive").parameter<bool>(0));

    // again move to active area
    m_subject->mouseMoveEvent(moveEvent);
    QCOMPARE(m_subject->dragAndDropState, (int)LockScreenWithPadlockView::STATE_MOVING_ACTIVE);
    QVERIFY(nameOfLastFeedback == "enter-dragndrop-dropzone");
    QVERIFY(gUnlockAreaStub->stubLastCallTo("setActive").parameter<bool>(0));

    // when the mouse is released, the unlocked signal should be sent
    m_subject->mouseReleaseEvent(releaseEvent);
    QCOMPARE(unlockSpy.count(), 1);
    QVERIFY(nameOfLastFeedback == "release-inside-dragndrop-dropzone");
    QCOMPARE(m_subject->dragAndDropIcon->objectName(), QString("LockScreenDnDIcon"));
    QVERIFY(!m_subject->dragAndDropOverlay.isVisible());
    QCOMPARE(m_subject->lockScreenHeader->objectName(), QString("LockLiftAreaWithPadlock"));
    QVERIFY(!gUnlockAreaStub->stubLastCallTo("setEnabled").parameter<bool>(0));
}

void Ut_LockScreenWithPadlockView::testNotificationAreaVisibility()
{
    QVERIFY(!m_subject->notificationArea->isVisible());
    QVERIFY(m_subject->notificationArea->parentLayoutItem() == 0);
    m_subject->showHideNotifications(true);
    QVERIFY(m_subject->notificationArea->isVisible());
    QVERIFY(m_subject->notificationArea->parentLayoutItem() == m_subject->layout);
}

void Ut_LockScreenWithPadlockView::testCreatedItemsAreRemovedFromTheControllerAndTheScene()
{
    controller->setView(NULL);

    // All the items added by the view should have disappeared
    QCOMPARE(controller->childItems().count(), 0);

    // All the SceneWindows should be gone as well
    QCOMPARE(gVisibleSceneWindows.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_LockScreenWithPadlockView)
