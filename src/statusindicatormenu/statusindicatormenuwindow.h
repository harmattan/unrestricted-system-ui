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

#ifndef STATUSINDICATORMENUWINDOW_H
#define STATUSINDICATORMENUWINDOW_H

#include <MWindow>
#include <MSceneWindow>
#include <MOverlay>
#include <mstatusbar.h>
#include "mstatusindicatormenuextensioninterface.h"
#include "mstatusbar.h"

class QGraphicsSceneMouseEvent;
class QGraphicsLinearLayout;
class MPannableViewport;
class NotificationArea;
class StatusIndicatorMenuWindow;
class MPannableViewport;

/*!
  * Overlay widget to be added to close button overlay of M Status Indicator Menu Window
  * which will not allow mouse events to pass through.
  */
class EventEaterWidget : public QGraphicsWidget
{
public:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end
};

/*!
  * A special panned widget for status indicator menu.
  * - Emits a signal when a geometry change happens.
  * - Emits a signal when a mouse press event is received below
  *   the bottommost widget.
  */
class PannedWidgetController : public MWidgetController
{
    Q_OBJECT
public:
    /*!
     * Constructs a layout change request listener widget
     *
     * \param parent the parent QGraphicsItem
     */
    PannedWidgetController(QGraphicsItem *parent = NULL);

    /*!
     * Gets the widget that is considered as the bottommost inside this
     * widget.
     * \return widget the bottommost widget.
     */
    const QGraphicsWidget *bottommostWidget() const;

    /*!
     * Sets the widget that is considered as the bottommost inside this
     * widget. The pressedOutSideContents signal is emitted if a press event
     * is received below this widget.
     * \param widget the bottom most widget.
     */
    void setBottommostWidget(const QGraphicsWidget *widget);

    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

signals:
    /*!
     * Sent when the position or the size of the item changes
     */
    void positionOrSizeChanged();

    /*!
     * A signal that gets emitted when the widget is pressed outside of its contents.
     */
    void pressedOutSideContents();

private:
    //! The bottom most widget inside this widget
    const QGraphicsWidget *bottommostWidget_;
};

/*!
 * Main window for the M Status Indicator Menu.
 * The window contains an application page which is populated
 * with a list of plugins.
 */
class StatusIndicatorMenuWindow : public MWindow, public MStatusIndicatorMenuInterface
{
    Q_OBJECT

public:
    /*!
     * Constructs a main window for the M Status Indicator Menu.
     *
     * \param parent Parent for the widget, defaults to NULL
     */
    StatusIndicatorMenuWindow(QWidget *parent = NULL);

    /*!
     * Destructor
     */
    ~StatusIndicatorMenuWindow();

    /*!
     * Makes the window visible by showing or raising it.
     */
    void makeVisible();

    //! \reimp
    virtual void showStatusIndicatorMenu();
    //! \reimp_end

signals:
    /*!
     * Signal the current visility status of window
     * \param visible true when window is top and false when window is obscured
     */
    void visibilityChanged(bool visible);

public slots:
    //! \reimp
    virtual void hideStatusIndicatorMenu();
    //! \reimp_end

private slots:
    //! Slot when window becomes top window
    void displayActive();

    //! Slot when window is no longer top window
    void displayInActive();

    /*!
     * \brief Sets the status indicator menu interface for the application extensions
     */
    void setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension);

    /*!
     * \brief Slot for getting information about settings button clicks
     */
    void launchControlPanelAndHide();

    //! Set the pannability and layout based on the size and position of the pannable area
    void setPannabilityAndLayout();

private:

    //! The name of the control panel service
    const static QString CONTROL_PANEL_SERVICE_NAME;

    //! The main scene window
    MSceneWindow *sceneWindow;

    //! Status Bar
    MStatusBar *statusBar;

    //! The pannable area viewport
    MPannableViewport *pannableViewport;

    //! An overlay for the close button
    MOverlay *closeButtonOverlay;

    //! Creates windows top row containing extension area and settings button
    QGraphicsWidget *createTopRow();

    //! Creates pannable area
    MPannableViewport *createPannableArea();

    //! Creates close button row
    QGraphicsWidget *createCloseButtonRow();

    //! Creates close button overlay
    MOverlay *createCloseButtonOverlay();

    //! Pannable area background widget
    MWidgetController *backgroundWidget;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicatorMenuWindow;
#endif
};

#endif // STATUSINDICATORMENUWINDOW_H
