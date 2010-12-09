/***************************************************************************
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

#ifndef STATUSINDICATORMENUBUSINESSLOGIC_H_
#define STATUSINDICATORMENUBUSINESSLOGIC_H_

#include <QObject>

class StatusIndicatorMenuWindow;

/*!
 * Status indicator menu business logic handles creation and
 * destruction of the status indicator menu.
 */
class StatusIndicatorMenuBusinessLogic : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs status indicator menu business logic.
     *
     * \param parent the parent QObject
     */
    StatusIndicatorMenuBusinessLogic(QObject *parent = NULL);

    /*!
     * Destroys the status indicator menu business logic.
     */
    virtual ~StatusIndicatorMenuBusinessLogic();

    /*!
     * Shows the status indicator menu and creates it if it does not exist yet.
     */
    void showStatusIndicatorMenu();

    /*!
     * Returns \c true if the status indicator menu is visible.
     *
     * \return \c true if the status indicator menu is visible, \c false otherwise
     */
    bool isStatusIndicatorMenuVisible() const;

signals:
    /*!
     * Signals the current visibility of the status indicator menu window.
     *
     * \param visible true when window is top and false when window is obscured
     */
    void statusIndicatorMenuVisibilityChanged(bool visible);

private:
    //! Status indicator menu
    StatusIndicatorMenuWindow *statusIndicatorMenuWindow;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicatorMenuBusinessLogic;
#endif
};

#endif /* STATUSINDICATORMENUBUSINESSLOGIC_H_ */
