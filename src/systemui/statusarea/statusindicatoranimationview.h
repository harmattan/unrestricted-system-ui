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
#include "statusindicatoriconstyle.h"
#include <MWidgetView>

class QPixmap;
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

    //! \reimp
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

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
    void setAnimationFrame(int frame);

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

    /*!
     * Sets up the animation timeline
     */
    void setupAnimationTimeline();

    /*!
     * Sets the first frame for the animation
     * \param frame the animation frame from which the animation starts
     */
    void setFirstAnimationFrame(int frame);

    /*!
     * Clears the images list
     */
    void clearImageList();

    /*!
     * Sets up the images list from an icon ID string
     *
     * \param iconIDs a list of icon IDs to create the image list from
     */
    void setupImageList(const QString &iconIDs);


    //! The controller for this view
    StatusIndicator *controller;

    //! The list of names of the images to be shown
    QStringList imageList;

    //! A vector of images to be shown
    mutable QVector<const QPixmap *> images;

    //! The current animation frame
    int animationFrame;

    //! The animation frame from which the animation starts
    int firstAnimationFrame;

    //! Timeline for the animation
    QTimeLine *animationTimeline;

private:

    //! Loads the current animation frame if not already loaded
    void loadCurrentFrame();
    //! Resizes this view according to the current pixmap shown if needed.
    void resizeToCurrentFrameIfNeeded();

#ifdef UNIT_TEST
    friend class Ut_StatusIndicatorAnimationView;
#endif
};

#endif
