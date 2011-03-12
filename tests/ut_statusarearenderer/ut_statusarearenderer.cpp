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
#include "x11wrapper_stub.h"
#include <QX11Info>

const static Window MEEGOTOUCH_STATUSBAR_WINDOW_ID = 1;
#define ATOM_MEEGOTOUCH_STATUSBAR_PIXMAP 0x00000001
#define ATOM_MEEGOTOUCH_STATUSBAR_WINDOW_PROPERTY 0x00000002

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

Pixmap X11Wrapper::XCreatePixmap(Display *display, Drawable drawable, unsigned int width, unsigned height, unsigned int depth)
{
    return ::XCreatePixmap(display, drawable, width, height, depth);
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

    gX11WrapperStub->stubReset();
    gX11WrapperStub->stubSetReturnValue("XCreateWindow", MEEGOTOUCH_STATUSBAR_WINDOW_ID);

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
    if(statusAreaRenderer) {
        delete statusAreaRenderer;
        statusAreaRenderer = NULL;
    }
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
    QVERIFY(handle != 0);
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

void Ut_StatusAreaRenderer::testStatusBarPropertyWindowCreation()
{
    Window rootWindow = DefaultRootWindow(QX11Info::display());

    QCOMPARE(gX11WrapperStub->stubCallCount("XCreateWindow"), 1);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XCreateWindow").parameter<Window>(1), rootWindow);
    QCOMPARE((int)gX11WrapperStub->stubLastCallTo("XCreateWindow").parameter<unsigned int>(8), InputOnly);
}

void Ut_StatusAreaRenderer::testStatusBarPixmapPropertiesAreSetCorrectly()
{
    Window rootWindow = DefaultRootWindow(QX11Info::display());

    QCOMPARE(gX11WrapperStub->stubCallCount("XInternAtom"), 2);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XInternAtom").at(0)->parameter<const char*>(1), "_MEEGOTOUCH_STATUSBAR_PIXMAP");
    QCOMPARE(gX11WrapperStub->stubCallsTo("XInternAtom").at(1)->parameter<const char*>(1), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW");
    QCOMPARE(gX11WrapperStub->stubCallCount("XChangeProperty"), 2);

    // Verify setting pixmap property
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Window>(1), MEEGOTOUCH_STATUSBAR_WINDOW_ID);
    QCOMPARE((int)gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Atom>(2), ATOM_MEEGOTOUCH_STATUSBAR_PIXMAP);
    QCOMPARE((ulong)gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Atom>(3), XA_PIXMAP);

    // Verify setting property window property
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Window>(1), rootWindow);
    QCOMPARE((int)gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Atom>(2), ATOM_MEEGOTOUCH_STATUSBAR_WINDOW_PROPERTY);
    QCOMPARE((ulong)gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Atom>(3), XA_WINDOW);
}

void Ut_StatusAreaRenderer::testStatusBarPixmapPropertiesAreDeletedInDestructor()
{
    delete statusAreaRenderer;
    statusAreaRenderer = NULL;

    // Check XDeleteProperty deletes the correct property
    QCOMPARE(gX11WrapperStub->stubCallCount("XDeleteProperty"), 2);
    QCOMPARE((int)gX11WrapperStub->stubCallsTo("XDeleteProperty").at(0)->parameter<Atom>(2), ATOM_MEEGOTOUCH_STATUSBAR_WINDOW_PROPERTY);
    QCOMPARE((int)gX11WrapperStub->stubCallsTo("XDeleteProperty").at(1)->parameter<Atom>(2), ATOM_MEEGOTOUCH_STATUSBAR_PIXMAP);
}

QTEST_MAIN(Ut_StatusAreaRenderer)
