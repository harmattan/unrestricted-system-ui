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

#include <DuiEscapeButtonPanel>
#include <DuiWindow>
#include <DuiApplicationPage>

/*!
 * Main window for the DUI Status Indicator Menu.
 * The window contains an application page which is populated
 * with a list of plugins.
 */
class StatusIndicatorMenuWindow : public DuiWindow
{
    Q_OBJECT

public:
    /*!
     * Constructs a main window for the DUI Status Indicator Menu.
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

signals:
    /*!
     * Signal the current visility status of window
     * \param visible true when window is top and false when window is obscured
     */
    void visibilityChanged(bool visible);

private slots:
    //! Slot when window becomes top window
    void displayActive();
    //! Slot when window is no longer top window
    void displayInActive();

private:
    /*!
     * Sets the X window properties for the window so that the window is not
     * included in the task bar.
     */
    void excludeFromTaskBar();

    //! The main application page
    QSharedPointer<DuiApplicationPage> applicationPage;

    //! The escape button
    QSharedPointer<DuiEscapeButtonPanel> escapeButtonPanel;
};

#endif // STATUSINDICATORMENUWINDOW_H
