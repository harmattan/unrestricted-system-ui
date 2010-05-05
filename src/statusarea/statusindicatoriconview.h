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

#ifndef STATUSINDICATORICONVIEW_H
#define STATUSINDICATORICONVIEW_H

#include "statusindicator.h"
#include "statusindicatormodel.h"
#include "statusindicatoriconstyle.h"
#include <MWidgetView>

class QPixmap;

/*!
 * \class StatusIndicatorIconView
 * \brief a view class for the status indicator widgets.
*/
class StatusIndicatorIconView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(StatusIndicatorModel, StatusIndicatorIconStyle)

public:
    /*!
     * \brief Constructs a new StatusIndicatorIconView.
     *
     * \param controller The view's controller
     */
    StatusIndicatorIconView(StatusIndicator *controller);

    /*!
     * \brief Destroys the StatusIndicatorIconView.
     */
    virtual ~StatusIndicatorIconView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! The controller for this view
    StatusIndicator *controller;

    //! The pixmap to be drawn
    const QPixmap *pixmap;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicatorIconView;
#endif
};

#endif
