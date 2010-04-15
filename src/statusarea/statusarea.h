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

#ifndef STATUSAREA_H
#define STATUSAREA_H

#include "statusindicatormenuwindow.h"
#include "statusareamodel.h"

#include <MButton>
#include <QSharedPointer>

class StatusAreaWindow;

/*!
 * Status area is the top part on the home screen. It contains the clock,
 * indicator area and a search button.
 */
class StatusArea : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(StatusArea)

public:
    /*!
     * Constructs a new StatusArea.
     *
     * \param parent the parent widget of the status area
     */
    StatusArea(MWidget *parent = NULL, StatusAreaWindow *statusAreaWindow = NULL);

    /*!
     * \brief Destructor
     */
    virtual ~StatusArea();

private:
    //! The name of the status indicator menu service
    static const QString STATUS_INDICATOR_MENU_SERVICE_NAME;
    //! Status indicator menu
    QSharedPointer<StatusIndicatorMenuWindow> statusIndicatorMenuWindow;

    /*!
     * Shows the status indicator menu when the user swipes the status area
     */
    void showStatusIndicatorMenu();

#ifdef UNIT_TEST
    friend class Ut_StatusArea;
#endif

};

#endif // STATUSAREA_H
