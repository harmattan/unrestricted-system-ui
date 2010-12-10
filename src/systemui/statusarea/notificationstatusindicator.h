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

#include <MWidgetController>
#include "statusindicator.h"

/*!
 * A status indicator for showing if there are new notifications
 */
class NotificationStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(NotificationStatusIndicator)

public:
    /*!
     * Constructs a NotificationStatusIndicator.
     * \param parent parent MWidget. If parent is NULL, the creating client must take the responsibility to delete the indicator.
     */
    explicit NotificationStatusIndicator(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the status indicator.
     */
    virtual ~NotificationStatusIndicator();

private slots:
    //! Slot to listen whether status indicator is active (displaying) or not
    void setActive(bool active);

private:
#ifdef UNIT_TEST
    friend class Ut_NotificationStatusIndicator;
#endif
};
