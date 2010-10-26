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

#ifndef NOTIFICATIONAREAVIEW_H_
#define NOTIFICATIONAREAVIEW_H_

#include <MWidgetView>

#include "notificationareastyle.h"
#include "notificationareamodel.h"

class NotificationArea;
class QGraphicsLinearLayout;
class MLayout;
class MLinearLayoutPolicy;
class MButton;

class NotificationAreaView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(NotificationAreaModel, NotificationAreaStyle)

public:
    /*!
     * Constructs a view for a notification area.
     *
     * \param controller the controller of this NotificationAreaView
     */
    NotificationAreaView(NotificationArea *controller);

    /*!
     * Destroys the notification area view.
     */
    virtual ~NotificationAreaView();

protected:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    virtual void applyStyle();
    //! \reimp_end

private:
    //! Updates the layout
    void updateLayout();

    //! The layout for the banners
    MLayout *bannerLayout;

    //! The layout policy for the banners
    MLinearLayoutPolicy *bannerPolicy;

    //! A layout for the clear button
    QGraphicsLinearLayout *clearButtonLayout;

    //! The clear button
    MButton *clearButton;

    //! And more
    MWidgetController *andMore;

#ifdef UNIT_TEST
    friend class Ut_NotificationAreaView;
#endif
};

#endif /* NOTIFICATIONAREAVIEW_H_ */
