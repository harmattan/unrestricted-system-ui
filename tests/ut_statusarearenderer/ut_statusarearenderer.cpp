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
#include <QtTest/QtTest>
#include "ut_statusarearenderer.h"
#include <MOnDisplayChangeEvent>
#include "statusarearenderer.h"
#include "statusarea_stub.h"
#include "statusareastyle.h"
#include <QMeeGoGraphicsSystemHelper>
#include "x11wrapper_modified_stub.h"
#include <QX11Info>
#include "xeventlistener_stub.h"
#include "xeventlistenerwidget_stub.h"

#define ROOT_WINDOW_ID (QX11Info::appRootWindow())
const static Window MEEGOTOUCH_STATUSBAR_WINDOW_ID = 1;
const static Window MEEGOTOUCH_WM_WINDOW_ID = 2;

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

const MStyle* MTheme::style(const char *, const QString &)
{
    return new MStyle;
}

void MTheme::releaseStyle(const MStyle *)
{
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

/* This stub adds asyncronous X errors emulation for XSelectInput calls.
 * For each Window argument passed to XSelectInput it is possible to define
 * an error that will be signalled via error handler set by the
 * XSetErrorHandler call. The stub also holds the window mask for each
 * Window argument of the successful XSelectInput calls (those that do not
 * signal an error).
 */
class X11WrapperWithErrors : public X11WrapperStub
{
public:
    X11WrapperWithErrors() : errorHandler(0) {};

    virtual XErrorHandler XSetErrorHandler(XErrorHandler handler)
    {
        QList<ParameterBase *> params;
        params.append(new Parameter<XErrorHandler >(handler));
        stubMethodEntered("XSetErrorHandler", params);

        XErrorHandler oldHandler = errorHandler;
        errorHandler = handler;

        return oldHandler;
    }

    virtual int XSelectInput(Display *display, Window w, long event_mask)
    {
        QList<ParameterBase *> params;
        params.append(new Parameter<Display * >(display));
        params.append(new Parameter<Window >(w));
        params.append(new Parameter<long >(event_mask));
        stubMethodEntered("XSelectInput", params);

        QHash<Window, unsigned char>::const_iterator i = errorForWindow.find(w);
        if( i != errorForWindow.end() ) {
            XErrorEvent err = { 0, display, 0, 0, i.value(), 0, 0 };
            errorHandler(display, &err);
        } else {
            // No error set for this window, update the event mask
            eventMask.insert(w, event_mask);
        }

        return stubReturnValue<int>("XSelectInput");
    }

    virtual void stubReset() const
    {
        X11WrapperStub::stubReset();

        errorHandler = 0;
        errorForWindow.clear();
        eventMask.clear();
    }

    mutable XErrorHandler errorHandler;
    mutable QHash<Window, unsigned char> errorForWindow;
    mutable QHash<Window, long> eventMask;
};

X11WrapperWithErrors gX11WrapperWithErrors;


bool wmPropertyStatusBarVisibleXGetReturnValue;
void Ut_StatusAreaRenderer::setupXGetPropertiesToDefault()
{
    wmPropertyStatusBarVisibleXGetReturnValue = true;

    XGetWindowPropertyDataReturnValueData returnData;
    returnData.type = XA_WINDOW;
    returnData.format = 32;
    returnData.data = (unsigned char*)&MEEGOTOUCH_WM_WINDOW_ID;
    returnData.return_value = Success;
    gX11WrapperStub->stubSetXGetWindowPropertyDataReturnValues(ROOT_WINDOW_ID, returnData);

    returnData.type = XA_CARDINAL;
    returnData.format = 32;
    returnData.data = (unsigned char *)&wmPropertyStatusBarVisibleXGetReturnValue;
    returnData.return_value = Success;
    gX11WrapperStub->stubSetXGetWindowPropertyDataReturnValues(MEEGOTOUCH_WM_WINDOW_ID, returnData);

    gX11WrapperStub->stubSetReturnValue("XGetWindowProperty", BadMatch);
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

    gX11WrapperStub = &gDefaultX11WrapperStub;
    gX11WrapperStub->stubReset();
    gX11WrapperStub->stubSetReturnValue("XCreateWindow", MEEGOTOUCH_STATUSBAR_WINDOW_ID);

    gXEventListenerStub->stubReset();
    gX11WrapperWithErrors.stubReset();

    setupXGetPropertiesToDefault();
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

void Ut_StatusAreaRenderer::testSetupStatusBarVisiblePropertyListening()
{
    QCOMPARE(statusAreaRenderer->windowManagerWindow, MEEGOTOUCH_WM_WINDOW_ID);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XFree").at(0)->parameter<void *>(0), (void *)&MEEGOTOUCH_WM_WINDOW_ID);

    QCOMPARE(statusAreaRenderer->statusBarVisible, true);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XFree").at(1)->parameter<void *>(0), (void *)&wmPropertyStatusBarVisibleXGetReturnValue);

    // Verify x event listening
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSelectInput").parameter<Window>(1), MEEGOTOUCH_WM_WINDOW_ID);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSelectInput").parameter<long>(2), PropertyChangeMask | StructureNotifyMask);
    QCOMPARE(gXEventListenerStub->stubCallCount("registerEventFilter"), 1);
    QCOMPARE(
            gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<XEventListenerFilterInterface *>(0),
            (XEventListenerFilterInterface *)statusAreaRenderer);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<long>(1), PropertyChangeMask | StructureNotifyMask);
}

void Ut_StatusAreaRenderer::testWhenStatusBarVisiblePropertySetupUnsuccessfulThenAssumingStatusBarVisible()
{
    delete statusAreaRenderer;
    gX11WrapperStub->stubSetReturnValue("XGetWindowProperty", BadWindow);
    statusAreaRenderer = new StatusAreaRenderer;

    QCOMPARE(statusAreaRenderer->statusBarVisible, true);
}

void Ut_StatusAreaRenderer::setStatusBarVisibility(bool visible)
{
    wmPropertyStatusBarVisibleXGetReturnValue = visible;
    XEvent event;
    event.xproperty.window = MEEGOTOUCH_WM_WINDOW_ID;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_VISIBLE", False);
    statusAreaRenderer->xEventFilter(event);
}

void Ut_StatusAreaRenderer::testStatusBarInvisibleStopsRendering()
{
    // "Turn" _MEEGOTOUCH_STATUSBAR_VISIBLE off
    setStatusBarVisibility(false);
    QCOMPARE(statusAreaRenderer->statusBarVisible, false);

    // Verify we are not rendering
    QList<QRectF> rectList;
    QRectF rect1(0,0,30,80);
    rectList.append(rect1);
    statusAreaRenderer->accumulateSceneChanges(rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, false);

    // "Turn" _MEEGOTOUCH_STATUSBAR_VISIBLE on
    setStatusBarVisibility(true);
    QCOMPARE(statusAreaRenderer->statusBarVisible, true);

    // Verify we are rendering again
    statusAreaRenderer->accumulateSceneChanges(rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, true);
}

void Ut_StatusAreaRenderer::testWhenStatusBarBecomesVisibleRenderAccumulatedChanges()
{
    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    setStatusBarVisibility(false);
    emit changed(*rectList);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, false);
    setStatusBarVisibility(true);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, true);
}

void Ut_StatusAreaRenderer::testUnregisteringEventFilterWhenDestroyed()
{
    delete statusAreaRenderer;
    statusAreaRenderer = NULL;

    QCOMPARE(gXEventListenerStub->stubCallCount("unregisterEventFilter"), 1);
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
    setupXGetPropertiesToDefault();
    statusAreaRenderer = new StatusAreaRenderer;
    QCOMPARE(statusAreaRenderer->renderScene, false);
}

void Ut_StatusAreaRenderer::testWhenDisplaySwitchedOnRenderAccumulatedChanges()
{
    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    emit displayStateChanged(MeeGo::QmDisplayState::Off);
    emit changed(*rectList);
    emit displayStateChanged(MeeGo::QmDisplayState::On);
    QCOMPARE(Ut_StatusAreaRenderer_Scene_Render_Called, true);
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
    QCOMPARE(statusAreaRenderer->backPixmap.size().height(), gStatusAreaSize.height());

    QCOMPARE(gPainterPaintDevices.at(0), gLivePixmapLockedImage);
    QCOMPARE(gPainterPaintDevices.at(1), &statusAreaRenderer->statusAreaPixmap);
    QCOMPARE(gPainterSourcePixmap, &statusAreaRenderer->backPixmap);
}

void Ut_StatusAreaRenderer::testRenderingWithoutMeeGo()
{
    delete statusAreaRenderer;
    gGraphicsSystemName = QLatin1String("not_meego");
    setupXGetPropertiesToDefault();
    statusAreaRenderer = new StatusAreaRenderer;

    QCOMPARE(statusAreaRenderer->statusAreaLivePixmap, static_cast<QMeeGoLivePixmap*>(0) );
    QCOMPARE(statusAreaRenderer->backPixmap.size().width(), gStatusAreaSize.width());
    QCOMPARE(statusAreaRenderer->backPixmap.size().height(), gStatusAreaSize.height());

    RenderTestsHelper helper;
    QList<QRectF>* rectList = helper.setupRenderTests(this, statusAreaRenderer);
    emit changed(*rectList);

    QCOMPARE(gPainterPaintDevices.at(0), &statusAreaRenderer->backPixmap);
    QCOMPARE(gPainterPaintDevices.at(1), &statusAreaRenderer->statusAreaPixmap);
    QCOMPARE(gPainterSourcePixmap, &statusAreaRenderer->backPixmap);
}

void Ut_StatusAreaRenderer::testStatusBarPropertyWindowCreation()
{
    QCOMPARE(gX11WrapperStub->stubCallCount("XCreateWindow"), 1);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XCreateWindow").parameter<Window>(1), ROOT_WINDOW_ID);
    QCOMPARE((int)gX11WrapperStub->stubLastCallTo("XCreateWindow").parameter<unsigned int>(8), InputOnly);
}

void Ut_StatusAreaRenderer::testStatusBarPixmapPropertiesAreSetCorrectly()
{
    QCOMPARE(gX11WrapperStub->stubCallCount("XInternAtom"), 6);
    QCOMPARE(gX11WrapperStub->stubCallCount("XChangeProperty"), 2);

    // Verify setting pixmap property
    QCOMPARE(gX11WrapperStub->stubCallsTo("XInternAtom").at(4)->parameter<const char*>(1), "_MEEGOTOUCH_STATUSBAR_PIXMAP");
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Window>(1), MEEGOTOUCH_STATUSBAR_WINDOW_ID);
    QCOMPARE((ulong)gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Atom>(3), XA_PIXMAP);

    // Verify setting property window property
    QCOMPARE(gX11WrapperStub->stubCallsTo("XInternAtom").at(5)->parameter<const char*>(1), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW");
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Window>(1), ROOT_WINDOW_ID);
    QCOMPARE((ulong)gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Atom>(3), XA_WINDOW);
}

void Ut_StatusAreaRenderer::testStatusBarPixmapPropertiesAreDeletedInDestructor()
{
    gX11WrapperStub->stubReset();

    delete statusAreaRenderer;
    statusAreaRenderer = NULL;

    // Check XDeleteProperty deletes the correct property
    QCOMPARE(gX11WrapperStub->stubCallCount("XDeleteProperty"), 2);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XInternAtom").at(0)->parameter<const char*>(1), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW");
    QCOMPARE(gX11WrapperStub->stubCallsTo("XInternAtom").at(1)->parameter<const char*>(1), "_MEEGOTOUCH_STATUSBAR_PIXMAP");
}

void Ut_StatusAreaRenderer::testStatusBarPixmapSharingIsAllowed()
{
    QCOMPARE(gX11WrapperStub->stubCallCount("XSendEvent"), 1);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<Display *>(0), QX11Info::display());
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<Window>(1), DefaultRootWindow(QX11Info::display()));
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<Bool>(2), False);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<long>(3), (long)0);

    XEvent event = gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<XEvent>(4);
    XClientMessageEvent *clientMessageEvent = (XClientMessageEvent *)&event;
    QCOMPARE(clientMessageEvent->type, ClientMessage);
    QCOMPARE(clientMessageEvent->window, (unsigned long)statusAreaRenderer->sharedPixmapHandle());
    QCOMPARE(clientMessageEvent->message_type, X11Wrapper::XInternAtom(QX11Info::display(), "XSERVER_SECURITY_POLICY_SHARE_DRAWABLE", False));
    QCOMPARE(clientMessageEvent->format, 8);
    QCOMPARE(clientMessageEvent->data.b[0], (char)True);
}

void Ut_StatusAreaRenderer::testWMWindowUnavailableInStartUp()
{
    delete statusAreaRenderer;
    statusAreaRenderer = NULL;

    gX11WrapperStub->stubReset();
    gXEventListenerStub->stubReset();
    gX11WrapperStub = &gX11WrapperWithErrors;
    gX11WrapperStub->stubSetReturnValue("XGetWindowProperty", BadMatch);
    gX11WrapperStub->stubSetReturnValue("XGetWindowAttributes", 1);

    statusAreaRenderer = new StatusAreaRenderer;

    // Verify that status bar visibility was reset
    QCOMPARE(statusAreaRenderer->statusBarVisible, true);
    // Verify registering the root window filter
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(0)->parameter<Window>(1), ROOT_WINDOW_ID);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(0)->parameter<long>(2), PropertyChangeMask);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<XEventListenerFilterInterface *>(0),
             (XEventListenerFilterInterface *)statusAreaRenderer);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<long>(1), PropertyChangeMask);

    // Verify that the "status bar visible" listener never attempted to register
    // since the window manager window property is absent from the root window
    QCOMPARE(gX11WrapperStub->stubCallCount("XSelectInput"), 1);
    QCOMPARE(gXEventListenerStub->stubCallCount("registerEventFilter"), 1);
}

void Ut_StatusAreaRenderer::testWMWindowBecomingUnavailable()
{
    gX11WrapperStub = &gX11WrapperWithErrors;
    gX11WrapperStub->stubReset();
    gXEventListenerStub->stubReset();

    setupXGetPropertiesToDefault();
    gX11WrapperStub->stubSetReturnValue("XGetWindowAttributes", 1);

    // Assume that status bar visibility was set "off" by the window manager
    statusAreaRenderer->statusBarVisible = false;

    // WM window becomes unavailable
    gX11WrapperWithErrors.errorForWindow.insert(MEEGOTOUCH_WM_WINDOW_ID, BadWindow);
    XEvent event;
    event.type = DestroyNotify;
    event.xdestroywindow.window = MEEGOTOUCH_WM_WINDOW_ID;
    statusAreaRenderer->xEventFilter(event);

    // Verify that status bar visibility was reset
    QCOMPARE(statusAreaRenderer->statusBarVisible, true);
    // Verify registering the root window filter
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(0)->parameter<Window>(1), ROOT_WINDOW_ID);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(0)->parameter<long>(2), PropertyChangeMask);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<XEventListenerFilterInterface *>(0),
             (XEventListenerFilterInterface *)statusAreaRenderer);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<long>(1), PropertyChangeMask);

    // Verify that the "status bar visible" listener tried, but failed to register
    QCOMPARE(gX11WrapperStub->stubCallCount("XSelectInput"), 2);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(1)->parameter<Window>(1), MEEGOTOUCH_WM_WINDOW_ID);
    QCOMPARE(gXEventListenerStub->stubCallCount("registerEventFilter"), 1);
}

void Ut_StatusAreaRenderer::testWMWindowBecomingAvailable_data()
{
    QTest::addColumn<bool>("statusBarVisibleAvailable");
    QTest::addColumn<bool>("statusBarVisibleValue");
    QTest::addColumn<bool>("statusBarVisibleResult");

    QTest::newRow("_MEEGOTOUCH_STATUSBAR_VISIBLE not set") << false << false << true;
    QTest::newRow("_MEEGOTOUCH_STATUSBAR_VISIBLE set to False") << true << false << false;
    QTest::newRow("_MEEGOTOUCH_STATUSBAR_VISIBLE set to True") << true << true << true;
}

void Ut_StatusAreaRenderer::testWMWindowBecomingAvailable()
{
    QFETCH(bool, statusBarVisibleAvailable);
    QFETCH(bool, statusBarVisibleValue);
    QFETCH(bool, statusBarVisibleResult);

    gX11WrapperStub = &gX11WrapperWithErrors;

    // WM window becomes unavailable
    gX11WrapperWithErrors.errorForWindow.insert(MEEGOTOUCH_WM_WINDOW_ID, BadWindow);
    XEvent event;
    event.type = DestroyNotify;
    event.xdestroywindow.window = MEEGOTOUCH_WM_WINDOW_ID;
    statusAreaRenderer->xEventFilter(event);

    // Reset counts
    gX11WrapperStub->stubReset();
    gXEventListenerStub->stubReset();

    // Set properties for the test case
    setupXGetPropertiesToDefault();
    wmPropertyStatusBarVisibleXGetReturnValue = statusBarVisibleValue;
    if(!statusBarVisibleAvailable) {
        XGetWindowPropertyDataReturnValueData noProperty = { 0, 0, 0, BadMatch };
        gX11WrapperStub->stubSetXGetWindowPropertyDataReturnValues(MEEGOTOUCH_WM_WINDOW_ID, noProperty);
    }

    // WM window available again (without the status bar visibility atom)
    XEvent propertyChangeEvent;
    propertyChangeEvent.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_SUPPORTING_WM_CHECK", False);
    propertyChangeEvent.xproperty.window = ROOT_WINDOW_ID;
    statusAreaRenderer->xEventFilter(propertyChangeEvent);

    // Verify that tracking root window property changes stopped
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(0)->parameter<Window>(1), ROOT_WINDOW_ID);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(0)->parameter<long>(2), statusAreaRenderer->previousRootWindowEventMask);
    // Verify that tracking WM window property changes started
    QCOMPARE(statusAreaRenderer->windowManagerWindow, MEEGOTOUCH_WM_WINDOW_ID);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(1)->parameter<Window>(1), MEEGOTOUCH_WM_WINDOW_ID);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XSelectInput").at(1)->parameter<long>(2), PropertyChangeMask | StructureNotifyMask);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<XEventListenerFilterInterface *>(0),
            (XEventListenerFilterInterface *)statusAreaRenderer);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("registerEventFilter").parameter<long>(1), PropertyChangeMask | StructureNotifyMask);
    // Verify that the status area visibile property was looked up
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XGetWindowProperty").parameter<Window>(1), MEEGOTOUCH_WM_WINDOW_ID);
    QCOMPARE(statusAreaRenderer->statusBarVisible, statusBarVisibleResult);
}

void Ut_StatusAreaRenderer::testBug285985Workaround()
{
    gX11WrapperStub = &gX11WrapperWithErrors;

    // WM window becomes unavailable
    gX11WrapperWithErrors.errorForWindow.insert(MEEGOTOUCH_WM_WINDOW_ID, BadWindow);
    XEvent event;
    event.type = DestroyNotify;
    event.xdestroywindow.window = MEEGOTOUCH_WM_WINDOW_ID;
    statusAreaRenderer->xEventFilter(event);

    // Verify that PropertyChangeMask is set on the root window
    QCOMPARE((gX11WrapperWithErrors.eventMask.value(ROOT_WINDOW_ID) & PropertyChangeMask), PropertyChangeMask);
    // WM starts and modifies the _NET_SUPORTED atom
    event.xproperty.window = ROOT_WINDOW_ID;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_SUPPORTED", False);
    statusAreaRenderer->xEventFilter(event);
    // The bug will reset the event mask
    long mask = gX11WrapperWithErrors.eventMask.value(ROOT_WINDOW_ID);
    gX11WrapperWithErrors.eventMask.insert(ROOT_WINDOW_ID,
                                           mask & ~PropertyChangeMask);
    // Let the application event loop spin so that the bug workaround has
    // an opportunity to handle the situition
    qApp->processEvents();
    // Verify that PropertyChangeMask is set on the root window
    QCOMPARE((gX11WrapperWithErrors.eventMask.value(ROOT_WINDOW_ID) & PropertyChangeMask), PropertyChangeMask);
}

void Ut_StatusAreaRenderer::testSetupStatusBarVisibleListenerFreesDataCorrectly_data()
{
    QTest::addColumn<Status>("callReturn");
    QTest::addColumn<Atom>("atomType");
    QTest::addColumn<int>("atomFormat");
    QTest::addColumn<Window>("atomData");
    QTest::addColumn<int>("freeCalls");

    QTest::newRow("Failed call") << BadMatch << XA_WINDOW << 32 << (Window)0 << 0;
    QTest::newRow("Atom correct") << Success << XA_WINDOW << 32 << (Window)0x11111111 << 1;
    QTest::newRow("Atom type incorrect") << Success << XA_INTEGER << 32 << (Window)0x22222222 << 1;
    QTest::newRow("Atom format incorrect") << Success << XA_WINDOW << 1 << (Window)0x33333333 << 1;
}

void Ut_StatusAreaRenderer::testSetupStatusBarVisibleListenerFreesDataCorrectly()
{
    QFETCH(Status, callReturn);
    QFETCH(Atom, atomType);
    QFETCH(int, atomFormat);
    QFETCH(Window, atomData);
    QFETCH(int, freeCalls);

    void* ptr = NULL;

    // Reset counts
    gX11WrapperStub->stubReset();
    gXEventListenerStub->stubReset();

    // XGetWindowProperty returns a (Window*) that will be dereferenced
    // so for all successful calls it needs to be a valid pointer
    if(callReturn == Success) {
        ptr = &atomData;
    }

    // Set up the property
    XGetWindowPropertyDataReturnValueData returnData;
    returnData.type = atomType;
    returnData.format = atomFormat;
    returnData.data = (uchar *)ptr;
    returnData.return_value = callReturn;
    gX11WrapperStub->stubSetXGetWindowPropertyDataReturnValues(ROOT_WINDOW_ID, returnData);

    // Run the code
    statusAreaRenderer->setupStatusBarVisibleListener();

    // Make sure the data was freed
    QCOMPARE(gX11WrapperStub->stubCallCount("XFree"), freeCalls);
    if(freeCalls) {
        QCOMPARE(gX11WrapperStub->stubLastCallTo("XFree").parameter<void*>(0),
                 ptr);
    }
}

void Ut_StatusAreaRenderer::testUpdateStatusBarVisiblePropertyFreesDataCorrectly_data()
{
    QTest::addColumn<Status>("callReturn");
    QTest::addColumn<Atom>("atomType");
    QTest::addColumn<int>("atomFormat");
    QTest::addColumn<uchar>("atomData");
    QTest::addColumn<int>("freeCalls");

    QTest::newRow("Failed call") << BadMatch << XA_CARDINAL << 8 << (uchar)0 << 0;
    QTest::newRow("Atom correct") << Success << XA_CARDINAL << 8 << (uchar)1 << 1;
    QTest::newRow("Atom type incorrect") << Success << XA_INTEGER << 8 << (uchar)1 << 1;
}

void Ut_StatusAreaRenderer::testUpdateStatusBarVisiblePropertyFreesDataCorrectly()
{
    QFETCH(Status, callReturn);
    QFETCH(Atom, atomType);
    QFETCH(int, atomFormat);
    QFETCH(uchar, atomData);
    QFETCH(int, freeCalls);

    void* ptr = NULL;

    // Reset counts
    gX11WrapperStub->stubReset();
    gXEventListenerStub->stubReset();

    // XGetWindowProperty returns a (Window*) that will be dereferenced
    // so for all successful calls it needs to be a valid pointer
    if(callReturn == Success) {
        ptr = &atomData;
    }

    // Set up the property
    XGetWindowPropertyDataReturnValueData returnData;
    returnData.type = atomType;
    returnData.format = atomFormat;
    returnData.data = (uchar *)ptr;
    returnData.return_value = callReturn;
    gX11WrapperStub->stubSetXGetWindowPropertyDataReturnValues(MEEGOTOUCH_WM_WINDOW_ID, returnData);

    // Run the code
    statusAreaRenderer->updateStatusBarVisibleProperty();

    // Make sure the data was freed
    QCOMPARE(gX11WrapperStub->stubCallCount("XFree"), freeCalls);
    if(freeCalls) {
        QCOMPARE(gX11WrapperStub->stubLastCallTo("XFree").parameter<void*>(0),
                 ptr);
    }
}

QTEST_MAIN(Ut_StatusAreaRenderer)
