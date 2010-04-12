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

#ifndef STATUSAREAWINDOW_H_
#define STATUSAREAWINDOW_H_

#include <DuiWindow>
#include <DuiNamespace>
#include "qmdisplaystate.h"

using namespace Maemo;

class QGraphicsScene;
class StatusArea;

/*!
 * Creates a window which contains a status area.
 */
class StatusAreaWindow : public DuiWindow
{
    Q_OBJECT

public:
    /*!
     * Creates a new status area window
     *
     * \param statusArea the status area to be displayed in this window
     * \param parent the parent widget
     */
    StatusAreaWindow(QWidget *parent = NULL);

    /*!
     * Destroys the status area window
     */
    virtual ~StatusAreaWindow();

    /*!
     * Returns the status area of this window.
     *
     * \return the status area of this window
     */
    StatusArea *statusArea() const;

    /*!
     * Initializes the status area window. Currently initializes scene
     */
    bool init();

private slots:
    /*!
     * \brief Rotates the status area to a new orientation
     *
     * \param angle the new angle
     */
    void rotate(const Dui::OrientationAngle &angle);

   /*!
    * \brief A slot for notifying that the scene has changed and needs to be painted
    */
    virtual void sceneChanged(const QList<QRectF> &region);

    /*!
     * \brief A slot for setting if the scene renders to pixmap or not
     */
    void setSceneRender(Maemo::QmDisplayState::DisplayState);

signals:
    /*!
     * Signal that status indicator menu is visible
     */
    void statusIndicatorMenuVisibilityChanged(bool visible);

private:
    //! Scene for this window
    QGraphicsScene *scene;

    //! The status area to be displayed in this window
    StatusArea *statusArea_;

    //! Shared Pixmap between libdui and systemui for the status area.
    QPixmap* statusAreaPixmap;

    //! Initializes the scene for use
    void initializeScene();

    //! Logical ID of shared pixmap which is used for rendering the status area
    static QString STATUS_AREA_PIXMAP_LOGICAL_ID;

    //! Keep track of device display state
    QmDisplayState* displayState;

    //! Keep track whether scene should render or not
    bool renderScene;
};

#endif /* STATUSAREAWINDOW_H_ */
