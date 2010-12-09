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

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

class MStatusBar;
class StatusIndicatorMenu;

/*!
 * Main window for the M Status Indicator Menu.
 * The window contains an application page which is populated
 * with a list of plugins.
 */
class StatusIndicatorMenuWindow : public MWindow
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

protected:
    //! \reimp
    bool event(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    //! \reimp_end

public slots:
    /*!
     * Makes the window visible by showing or raising it.
     */
    void makeVisible();

signals:
    /*!
     * Signals the current visibility of the status indicator menu window.
     *
     * \param visible true when window is top and false when window is obscured
     */
    void visibilityChanged(bool visible);

private slots:

    //! Slot when window becomes top window
    void displayActive();

    //! Slot when window is no longer top window
    void displayInActive();

#ifdef HAVE_QMSYSTEM
    /*!
     * \brief Slot for receiving device lock status changes
     * \param what tells which lockstate has been changed, either Device or TouchScreen/Keyboard
     * \param how tells to which state lock has been changed, one of Locked, Unlocked or Unknown
     */
    void setWindowStateAccordingToDeviceLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
#endif

private:
    /*! Resets menu widget.
     *
     * Dismisses window from scene manager, if window is appeared there.
     * Creates a new menu widget and appears window to scene manager.
     */
    void resetMenuWidget();

    //! Status bar
    MStatusBar *statusBar;

    //! The actual menu widget
    StatusIndicatorMenu *menuWidget;

    //! Current language
    QString currentLanguage;

    //! Position of the previous mouse press event
    QPoint mousePressPosition;

#ifdef HAVE_QMSYSTEM
    //! QmSystem watcher for device lock
    MeeGo::QmLocks qmLocks;

    //! Device lock status
    bool deviceLocked;
#endif

#ifdef UNIT_TEST
    friend class Ut_StatusIndicatorMenuWindow;
#endif
};

#endif // STATUSINDICATORMENUWINDOW_H
