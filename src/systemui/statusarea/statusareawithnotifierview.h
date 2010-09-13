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

#ifndef STATUSAREAWITHNOTIFIER_H
#define STATUSAREAWITHNOTIFIER_H

#include "statusareaview.h"

#include "statusareastyle.h"
#include "statusareamodel.h"
#include "contextframeworkcontext.h"


/*!
 * Status area view draws the status area.
 */
class StatusAreaWithNotifierView : public StatusAreaView
{
    Q_OBJECT
    M_VIEW(StatusAreaModel, StatusAreaStyle)

public:
   /*!
     * Constructs a view for a status area button.
     *
     * \param controller the controller of this StatusAreaView
     */
    StatusAreaWithNotifierView(StatusArea *controller);

    /*!
     * Destructor.
     */
    virtual ~StatusAreaWithNotifierView();

private:

    //! Notification indicator
    StatusIndicator *landscapeNotificationIndicator;
    StatusIndicator *portraitNotificationIndicator;

    //! sets up object names for functional testing
    void setupTestabilityObjectNames();

    //! sets up parents relationship for functional testing
    void setupTestabilityParents();

    //! set up class for functional testing
    void setupTestability();

#ifdef UNIT_TEST
    friend class Ut_StatusAreaWithNotifierView;
#endif
};
#endif // STATUSAREAWITHNOTIFIER_H
