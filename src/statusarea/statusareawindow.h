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

#include <MWindow>
#include <MNamespace>

class QGraphicsScene;
class StatusArea;

/*!
 * Creates a window which contains a status area. This window is not a top level window and is not visible.
 *  It renders the contents of the scene to a shared pixmap which is then shown by libmeegotouch.
 */
class StatusAreaWindow : public MWindow
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

private slots:
   /*!
    * \brief A slot for notifying that the scene has changed and needs to be painted
    */
    virtual void sceneChanged(const QList<QRectF> &region);

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

    //! Shared Pixmap between libmeegotouch and systemui for the status area.
    QPixmap* statusAreaPixmap;

    //! creates a pixmap and writes the handle to a temp file
    bool createSharedPixmapHandle();

    //! set the status bar size with information from style
    void setSizeFromStyle();

    //! Status Area dimensions.
    uint statusAreaHeight;
    uint statusAreaWidth;

#ifdef UNIT_TEST
    friend class Ut_StatusAreaWindow;
#endif

};

#endif /* STATUSAREAWINDOW_H_ */
