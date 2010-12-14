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

#ifndef STATUSINDICATORMENUDROPDOWNVIEW_H_
#define STATUSINDICATORMENUDROPDOWNVIEW_H_

#include <MSceneWindow>
#include <MStylableWidget>
#include <MOverlay>
#include <mscenewindowview.h>
#include "statusindicatormenustyle.h"

class QGraphicsSceneMouseEvent;
class QGraphicsLinearLayout;
class MPannableViewport;
class NotificationArea;
class MPannableViewport;
class MApplicationExtensionArea;
class StatusIndicatorMenuWindow;
class StatusIndicatorMenu;

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
class PannedWidgetController : public MStylableWidget
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
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
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




class StatusIndicatorMenuDropDownView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, StatusIndicatorMenuStyle)

public:
    /*!
     * Constructs a view for a notification area.
     *
     * \param controller the controller of this StatusIndicatorMenuDropDownView
     */
    StatusIndicatorMenuDropDownView(StatusIndicatorMenu *controller);

    /*!
     * Destroys the notification area view.
     */
    virtual ~StatusIndicatorMenuDropDownView();

    //! Delay for the initialization of the view (in milliseconds)
    static const int VIEW_INITIALIZATION_DELAY;

protected:
    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

private slots:
    //! Ensures that everything is ready for the window to be shown.
    void ensureIsViewable();

    //! Set the pannability and layout based on the size and position of the pannable area
    void setPannabilityAndLayout();

    //! Reset the viewport to the beginning of the pannable range
    void resetViewport();

private:

    //! The controller
    StatusIndicatorMenu *controller;

    //! The application extension area for the settings plugins
    MApplicationExtensionArea *settingsPluginsExtensionArea;

    //! The application extension area for the status indicator widgets
    MApplicationExtensionArea *statusIndicatorExtensionArea;

    //! Extension area for call ui and transfer ui plugins
    MApplicationExtensionArea* createVerticalExtensionArea();

    //! The pannable area viewport
    MPannableViewport *pannableViewport;

    //! An overlay for the close button
    MOverlay *closeButtonOverlay;

    //! The widget containing the top row of buttons
    QGraphicsWidget *topRowWidget;

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
    friend class Ut_StatusIndicatorMenuDropDownView;
#endif
};

#endif /* STATUSINDICATORMENUDROPDOWNVIEW_H_ */
