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
 * Creates a window which contains a status area.
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

    /*!
     * Returns the status area of this window.
     *
     * \return the status area of this window
     */
    StatusArea *statusArea() const;

private slots:
    /*!
     * \brief Rotates the status area to a new orientation
     *
     * \param angle the new angle
     */
    void rotate(const M::OrientationAngle &angle);

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
};

#endif /* STATUSAREAWINDOW_H_ */
