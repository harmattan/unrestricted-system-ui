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

#ifndef STATUSAREARENDERER_H_
#define STATUSAREARENDERER_H_

#include <MNamespace>
#include <QTimer>
#include <MOnDisplayChangeEvent>
#include "statusarea.h"
#include "xeventlistener.h"

#ifdef HAVE_QMSYSTEM
#include "qmdisplaystate.h"
#endif

class QGraphicsScene;
class StatusArea;
class QMeeGoLivePixmap;

/*!
 *  StatusAreaRenderer renders the contents of the scene to a shared pixmap which is then shown by libmeegotouch.
 */
class StatusAreaRenderer : public QObject, XEventListenerFilterInterface
{
    Q_OBJECT

public:
    /*!
     * Creates a new status area renderer
     *
     * \param parent the parent widget
     */
    StatusAreaRenderer(QObject *parent = NULL);

    /*!
     * Destroys the status area renderer
     */
    virtual ~StatusAreaRenderer();

    /*!
     * Returns the handle to the status area shared pixmap
     * \return handle to shared pixmap
     */
    uint sharedPixmapHandle();

    /*!
     * X event filter for _MEEGOTOUCH_STATUSBAR_VISIBLE changes
     */
    bool xEventFilter(const XEvent &event);

signals:
    /*!
     * Emitted when _NET_SUPPORTED property on root window changes,
     * notifying interested parties that Qt will reset the PropertyChangeMask
     * on the root window input mask.
     */
    void netSupportedPropertyChanged();

private slots:

    /*!
    * \brief Combine the given regions into one and start a timer to draw the region
    */
    virtual void accumulateSceneChanges(const QList<QRectF> &region);

   /*!
    * \brief Render the changed region of the status area
    */
    void renderAccumulatedRegion();

#ifdef HAVE_QMSYSTEM
    /*!
     * \brief A slot for setting if the scene renders to pixmap or not.
     * \param MeeGo::QmDisplayState::DisplayState. When MeeGo::QmDisplayState::Off / MeeGo::QmDisplayState::Dimmed the scene does not render. When MeeGo::QmDisplayState::On scene renders to pixmap
     */
    void setSceneRender(MeeGo::QmDisplayState::DisplayState state);
#endif

    //! Handles situation when WM window gets unavailable
    void wmWindowUnavailable();

private:
    //! Start tracking root window property changes
    void startTrackingRootWindowProperties();

    //! Stop tracking root window property changes
    void stopTrackingRootWindowProperties();

    /*!
     * Set up listening to the status bar visibility property changes on the
     * WM window.
     *
     * \return true if the WM window was found and subscribing to property
     * changes succeeded. false otherwise.
     */
    bool setupStatusBarVisibleListener();

    /*!
     * Fetch the _MEEGOTOUCH_STATUSBAR_VISIBLE property values from the WM
     * window.
     *
     * \return true if the WM window was found and subscribing to property
     * changes succeeded. false otherwise.
     */
    bool updateStatusBarVisibleProperty();

    //! Creates a window for sharing shared pixmap handle.
    void createStatusAreaPropertyWindow();

    //! Sets the status area property window id to root window property.
    void setStatusAreaPropertyWindowIdToRootWindowProperty();

    //! Sets handle of status area pixmap to a status area property window property
    void setSharedPixmapHandleToWindowProperty();

    //! Scene which is rendererd
    QGraphicsScene *scene;

    //! The status area to be displayed in this window
    StatusArea *statusArea;

    //! Shared Pixmap between libmeegotouch and systemui for the status area.
    QPixmap statusAreaPixmap;
    //! Back buffer pixmap - the content is first rendered here and then copied to the shared pixmap
    QPixmap backPixmap;
    //! Used for better performance if MeeGo graphics system is available, wrapped by backPixmap
    QMeeGoLivePixmap* statusAreaLivePixmap;

    //! Creates a shared pixmap for status area
    bool createSharedPixmapHandle();

    //! Creates the back buffer pixmap
    bool createBackPixmap();

    //! set the status bar size with information from style
    void setSizeFromStyle();

    //! Window id for the status area property window.
    Window statusAreaPropertyWindow;
    //! Window id of the window manager window
    Window windowManagerWindow;
    //! Widget representing the window manager window
    QWidget *windowManagerWindowWidget;

    //! Status Area dimensions.
    uint statusAreaHeight;
    uint statusAreaWidth;

    //! Whether status bar is currently visible according to compositor
    bool statusBarVisible;

#ifdef HAVE_QMSYSTEM
    //! Keep track of device display state
    MeeGo::QmDisplayState* displayState;
#endif

    //! Keep track whether scene should render or not
    bool renderScene;

    //! A single shot timer started when the scene changes
    QTimer accumulationTimer;
    //! Changes in the scene are combined to this rect
    QRectF accumulatedRegion;

    //! _MEEGOTOUCH_STATUSBAR_VISIBLE atom
    Atom statusBarVisibleAtom;
    //! _NET_SUPPORTING_WM_CHECK atom
    Atom windowManagerWindowAtom;
    //! _NET_SUPPORTED atom
    Atom netSupportedAtom;
    //! XSERVER_SECURITY_POLICY_SHARE_DRAWABLE atom
    Atom shareDrawableAtom;

    //! Root window property mask that was set before XSelectInput
    long previousRootWindowEventMask;

    //! An custom X error handler to collect error coders of asynchronous errors
    static int handleXError(Display* dpy, XErrorEvent *event);

    //! The error code of the last trapped X error
    static unsigned char xErrorCode;
    //! The previous X error handler
    static XErrorHandler oldXErrorHandler;

    //! Install X error handler to collect error codes
    void trapXErrors();

    //! Remove X error handler
    void untrapXErrors();

#ifdef UNIT_TEST
    friend class Ut_StatusAreaRenderer;
#endif

};

#endif /* STATUSAREARENDERER_H_ */
