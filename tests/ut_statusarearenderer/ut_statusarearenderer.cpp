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
** of this file .
**
****************************************************************************/
#include "ut_statusarearenderer.h"
#include <MOnDisplayChangeEvent>
#include "statusarearenderer.h"
#include "statusarea_stub.h"
#include "statusareastyle.h"
#include <QMeeGoGraphicsSystemHelper>

bool Ut_StatusAreaRenderer_Scene_Render_Called = false;
bool Ut_StatusAreaRenderer_Scene_SendEvent_Called = false;
QRectF rectReceived(0,0,0,0);
QEvent::Type eventReceived;
MOnDisplayChangeEvent::State eventStateReceived;
bool Ut_StatusAreaRenderer_syncX_Called = false;

QList<QGraphicsItem *> item_list;

void QApplication::syncX()
{
    Ut_StatusAreaRenderer_syncX_Called = true;
}

const MStyle* MTheme::style(const char *styleClassName,
                            const QString &objectName,
                            const QString &mode,
                            const QString &type,
                            M::Orientation orientation,
                            const MWidgetController *parent)
{
    Q_UNUSED(styleClassName);
    Q_UNUSED(objectName);
    Q_UNUSED(mode);
    Q_UNUSED(type);
    Q_UNUSED(orientation);
    Q_UNUSED(parent);
    return new MStyle;
}

MStyle::MStyle()
{
}

#ifdef HAVE_QMSYSTEM
MeeGo::QmDisplayState::DisplayState displayState = MeeGo::QmDisplayState::On;
MeeGo::QmDisplayState::DisplayState MeeGo::QmDisplayState::get() const
{
    return displayState;
}
#endif

void QGraphicsScene::removeItem(QGraphicsItem *item)
{
    item_list.removeOne(item);
}

void QGraphicsScene::addItem(QGraphicsItem *item)
{
    item_list.append(item);
}

QList<QGraphicsItem *> QGraphicsScene::items() const
{
    return item_list;
}

void QGraphicsScene::render(QPainter *painter, const QRectF &target, const QRectF &source, Qt::AspectRatioMode aspectRatioMode)
{
    Ut_StatusAreaRenderer_Scene_Render_Called = true;
    rectReceived = source;
    Q_UNUSED(target);
    Q_UNUSED(painter);
    Q_UNUSED(aspectRatioMode);
}

bool QGraphicsScene::sendEvent(QGraphicsItem *item, QEvent *event)
{
    Ut_StatusAreaRenderer_Scene_SendEvent_Called = true;
    eventReceived = event->type();
    if (eventReceived == MOnDisplayChangeEvent::eventNumber()) {
        eventStateReceived = (static_cast<MOnDisplayChangeEvent*>(event))->state();
    }
    Q_UNUSED(item);
    return true;
}

QSize gStatusAreaSize(30, 80);

const QSize& MWidgetStyle::preferredSize() const
{
    return gStatusAreaSize;
}

bool gTimerStarted = false;
bool gTimerEmitTimeoutImmediately = true;
bool gTimerActive = false;
int gTimerInterval;

void QTimer::start(int interval)
{
    gTimerStarted = true;
    gTimerInterval = interval;

    if (gTimerEmitTimeoutImmediately) {
        emit timeout();
    } else {
        // QTimer::isActive returns true if id != 0
        id = 1;
    }
}

bool gTimerStopped = false;
void QTimer::stop()
{
    gTimerStopped = true;
    id = 0;
}

QLatin1String gGraphicsSystemName("meego");

QString QMeeGoGraphicsSystemHelper::runningGraphicsSystemName()
{
    return gGraphicsSystemName;
}

bool QMeeGoGraphicsSystemHelper::isRunningMeeGo()
{
    return (runningGraphicsSystemName() == QLatin1String("meego"));
}

void QMeeGoGraphicsSystemHelper::switchToMeeGo()
{
}

void QMeeGoGraphicsSystemHelper::switchToRaster()
{
}

static QImage* gLivePixmapLockedImage = (QImage*)(0xfeedbeef);

bool QImage::isNull() const
{
    return false;
}

QImage* QMeeGoLivePixmap::lock(QMeeGoFenceSync*)
{
    return gLivePixmapLockedImage;
}

void QMeeGoLivePixmap::release(QImage*)
{
}

QList<QPaintDevice*> gPainterPaintDevices;

bool QPainter::begin(QPaintDevice *device)
{
    gPainterPaintDevices.append(device);

    return true;
}

bool QPainter::end()
{
    return true;
}

bool QPainter::isActive() const
{
    return true;
}

void QPainter::fillRect(const QRectF &, const QColor &)
{
}

const QPixmap* gPainterSourcePixmap;

void QPainter::drawPixmap(const QRectF &, const QPixmap &pixmap, const QRectF &)
{
    gPainterSourcePixmap = &pixmap;
}


void Ut_StatusAreaRenderer::init()
{
    gTimerStopped = false;
    gTimerStarted = false;
    gTimerEmitTimeoutImmediately = true;
    gTimerInterval = 0;

    gPainterPaintDevices.clear();
    gPainterSourcePixmap = NULL;

    gGraphicsSystemName = QLatin1String("meego");

    statusAreaRenderer = new StatusAreaRenderer();
}

void Ut_StatusAreaRenderer::cleanup()
{
    Ut_StatusAreaRenderer_syncX_Called = false;
    Ut_StatusAreaRenderer_Scene_Render_Called = false;
    Ut_StatusAreaRenderer_Scene_SendEvent_Called = false;
    rectReceived.setRect(0,0,0,0);

#ifdef HAVE_QMSYSTEM
    displayState = MeeGo::QmDisplayState::On;
#endif
    delete statusAreaRenderer;
}

void Ut_StatusAreaRenderer::initTestCase()
{
}

void Ut_StatusAreaRenderer::cleanupTestCase()
{
}

void Ut_StatusAreaRenderer::testSceneChanged()
{
    // Test an intersected rect
    QList<QRectF> rectList;
    QRectF rect1(0,0,30,80);
    rectList.append(rect1);
    connect(this, SIGNAL(changed(QList<QRectF>)), statusAreaRenderer, SLOT(accumulateSceneChanges(QList<QRectF>)));
    emit changed(rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, true);

    // Test when rect has no intersection with pixmap
    Ut_StatusAreaRenderer_Scene_Render_Called = false;
    Ut_StatusAreaRenderer_syncX_Called = false;
    rectList.pop_front();
    QRectF rect2(40,50,30,80);
    rectList.append(rect2);
    emit changed(rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, false);

    // Test when two rects called then scene render called with complete intersection rect( final rect should be Rect1 U Rect2 Intersectin sharedPixmapRect)
    rectList.pop_front();
    QRectF rect3(0,0,10,20);
    QRectF rect4(10,20,10,20);
    rectList.append(rect3);
    rectList.append(rect4);
    emit changed(rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, true);
    QRectF unitedRect = rect3.united(rect4) ;
    QRectF expectedRect = unitedRect.intersected(QRectF(0,0,30,80));
    QCOMPARE(rectReceived,expectedRect);
}

// Render test helper class definitions
void RenderTestsHelper::setupRect()
{
    delete rectList;
    rectList = new QList<QRectF>;
    QRectF rect1(0,0,30,80);
    rectList->append(rect1);
}

QList<QRectF>* RenderTestsHelper::setupRenderTests(Ut_StatusAreaRenderer* testClass, StatusAreaRenderer* statusAreaRenderer)
{
    setupRect();
    QObject::connect(testClass, SIGNAL(changed(QList<QRectF>)), statusAreaRenderer, SLOT(accumulateSceneChanges(QList<QRectF>)));
#ifdef HAVE_QMSYSTEM
    QObject::connect(testClass, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), statusAreaRenderer, SLOT(setSceneRender(MeeGo::QmDisplayState::DisplayState)));
#endif
    return rectList;
}

RenderTestsHelper::RenderTestsHelper() :
        rectList(NULL)
{
}

RenderTestsHelper::~RenderTestsHelper()
{
    delete rectList;
}

// end RenderTestsHelper
#ifdef HAVE_QMSYSTEM
void Ut_StatusAreaRenderer::testSceneRenderControlDisplayStateOn()
{
    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    emit displayStateChanged(MeeGo::QmDisplayState::On);
    emit changed(*rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, true);
}

void Ut_StatusAreaRenderer::testSceneRenderControlDisplayStateOff()
{
    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    emit displayStateChanged(MeeGo::QmDisplayState::Off);
    emit changed(*rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, false);
}

void Ut_StatusAreaRenderer::testSceneRenderControlDisplayStateDimmed()
{
    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    emit displayStateChanged(MeeGo::QmDisplayState::Dimmed);
    emit changed(*rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, false);
}

void Ut_StatusAreaRenderer::testMOnDisplayChangeEvent()
{
    // Initial condition
    RenderTestsHelper helper;
    helper.setupRenderTests(this, statusAreaRenderer);
    emit displayStateChanged(MeeGo::QmDisplayState::On);
    Ut_StatusAreaRenderer_Scene_SendEvent_Called = false;

    // Change to Dimmed state should produce a
    // MOnDisplayedChangeEvent with FullyOffDisplay state
    emit displayStateChanged(MeeGo::QmDisplayState::Dimmed);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_SendEvent_Called, true);
    QCOMPARE(eventReceived, MOnDisplayChangeEvent::eventNumber());
    QCOMPARE(eventStateReceived, MOnDisplayChangeEvent::FullyOffDisplay);

    // Change to On state should produce a
    // MOnDisplayedChangeEvent with FullyOnDisplay state
    Ut_StatusAreaRenderer_Scene_SendEvent_Called = false;
    emit displayStateChanged(MeeGo::QmDisplayState::On);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_SendEvent_Called, true);
    QCOMPARE(eventReceived, MOnDisplayChangeEvent::eventNumber());
    QCOMPARE(eventStateReceived, MOnDisplayChangeEvent::FullyOnDisplay);

    // Change to Off state should produce a
    // MOnDisplayedChangeEvent with FullyOffDisplay state
    Ut_StatusAreaRenderer_Scene_SendEvent_Called = false;
    emit displayStateChanged(MeeGo::QmDisplayState::Off);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_SendEvent_Called, true);
    QCOMPARE(eventReceived, MOnDisplayChangeEvent::eventNumber());
    QCOMPARE(eventStateReceived, MOnDisplayChangeEvent::FullyOffDisplay);
}

void Ut_StatusAreaRenderer::testSceneRenderControlWhenInitialDisplayStateOff()
{
    QCOMPARE(statusAreaRenderer->renderScene, true);
    delete statusAreaRenderer;
    displayState = MeeGo::QmDisplayState::Off;
    statusAreaRenderer = new StatusAreaRenderer;
    QCOMPARE(statusAreaRenderer->renderScene, false);
}

#endif

void Ut_StatusAreaRenderer::testSharedPixmapHandle()
{
    uint handle = static_cast<quint32> (statusAreaRenderer->statusAreaPixmap.handle());
    QCOMPARE(handle, statusAreaRenderer->sharedPixmapHandle());
}

void Ut_StatusAreaRenderer::testAccumulationTimer()
{
    gTimerEmitTimeoutImmediately = false;

    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);

    gTimerStarted = false;
    QCOMPARE(statusAreaRenderer->accumulationTimer.isActive(), false);

    emit changed(*rectList);
    QCOMPARE(gTimerStarted, true);
    QCOMPARE(statusAreaRenderer->accumulationTimer.isActive(), true);

    gTimerStarted = false;
    emit changed(*rectList);
    QCOMPARE(gTimerStarted, false);
    QCOMPARE(statusAreaRenderer->accumulationTimer.isActive(), true);
}

void Ut_StatusAreaRenderer::testRenderingWithMeeGo()
{
    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    emit changed(*rectList);

    QCOMPARE(statusAreaRenderer->backPixmap.handle(), statusAreaRenderer->statusAreaLivePixmap->handle());
    QCOMPARE(statusAreaRenderer->backPixmap.size().height(), gStatusAreaSize.height() * 2);

    QCOMPARE(gPainterPaintDevices.at(0), gLivePixmapLockedImage);
    QCOMPARE(gPainterPaintDevices.at(1), &statusAreaRenderer->statusAreaPixmap);
    QCOMPARE(gPainterSourcePixmap, &statusAreaRenderer->backPixmap);
}

void Ut_StatusAreaRenderer::testRenderingWithoutMeeGo()
{
    delete statusAreaRenderer;
    gGraphicsSystemName = QLatin1String("not_meego");
    statusAreaRenderer = new StatusAreaRenderer;

    QCOMPARE(statusAreaRenderer->statusAreaLivePixmap, static_cast<QMeeGoLivePixmap*>(0) );
    QCOMPARE(statusAreaRenderer->backPixmap.size().width(), gStatusAreaSize.width());
    QCOMPARE(statusAreaRenderer->backPixmap.size().height(), gStatusAreaSize.height() * 2);

    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    emit changed(*rectList);

    QCOMPARE(gPainterPaintDevices.at(0), &statusAreaRenderer->backPixmap);
    QCOMPARE(gPainterPaintDevices.at(1), &statusAreaRenderer->statusAreaPixmap);
    QCOMPARE(gPainterSourcePixmap, &statusAreaRenderer->backPixmap);
}

QTEST_MAIN(Ut_StatusAreaRenderer)
