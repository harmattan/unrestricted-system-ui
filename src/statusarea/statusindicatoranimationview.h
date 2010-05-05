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

#ifndef STATUSINDICATORANIMATIONVIEW_H
#define STATUSINDICATORANIMATIONVIEW_H

#include "statusindicator.h"
#include "statusindicatormodel.h"
#include "statusindicatoranimationstyle.h"
#include <MWidgetView>

class StatusIndicator;
class QPainter;
class QTimeLine;

/*!
 * \class StatusIndicatorAnimationView
 * \brief a view class for the status indicator widgets.
*/
class StatusIndicatorAnimationView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(StatusIndicatorModel, StatusIndicatorAnimationStyle)

public:
    /*!
     * \brief Constructs a new StatusIndicatorAnimationView.
     *
     * \param controller The view's controller
     */
    StatusIndicatorAnimationView(StatusIndicator *controller);

    /*!
     * \brief Destroys the StatusIndicatorAnimationView.
     */
    virtual ~StatusIndicatorAnimationView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void setupModel();
    //! \reimp_end

public slots:
    /*!
     * \brief A slot for starting the animation
     */
    virtual void startAnimation();

    /*!
     * \brief A slot for stopping the animation
     */
    virtual void stopAnimation();

    /*!
     * \brief A slot for setting the animation frame
     *
     * \param frame the animation frame to show
     */
    virtual void setAnimationFrame(int frame);

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    /*!
     * Clears the images list
     */
    void clearImageList();

    //! The controller for this view
    StatusIndicator *controller;

    //! The list of names of the images to be shown
    QStringList imageList;

    //! A vector of images to be shown
    mutable QVector<const QPixmap *> images;

    //! The current animation frame
    int animationFrame;

    //! Timeline for the animation
    QTimeLine *animationTimeline;
};

#endif
