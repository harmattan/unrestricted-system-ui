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
#ifndef LOCKSCREENVIEW_H
#define LOCKSCREENVIEW_H

#include <MWidgetView>
#include "lockscreenstyle.h"

class QGraphicsLinearLayout;
class MGConfItem;
class QPixmap;
class LockScreenHeader;

/*!
 * LockScreenBackgroundPixmap represents a background pixmap.
 */
class LockScreenBackgroundPixmap : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a background pixmap representation.
     *
     * \param gConfKey the name of the GConf key to get the pixmap name from
     */
    LockScreenBackgroundPixmap(const QString &gConfKey);

    /*!
     * Destroys the background pixmap representation.
     */
    ~LockScreenBackgroundPixmap();

    //! GConf item for the background image
    MGConfItem *gConfItem;

    //! The background pixmap
    QPixmap *pixmap;

    //! Whether the background pixmap is from the theme
    bool pixmapFromTheme;

private slots:
    //! Updates the pixmap based on the GConf key value
    void updatePixmap();

signals:
    //! Signaled when the pixmap is updated
    void pixmapUpdated();

private:
    //! Destroys the pixmap
    void destroyPixmap();

#ifdef UNIT_TEST
    friend class Ut_LockScreenView;
#endif
};

/*!
 * LockScreenView renders the background image of the lock screen.
 */
class LockScreenView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, LockScreenStyle)

public:
    /*!
     * Creates a lock screen view.
     *
     * \param controller controller for the view
     */
    LockScreenView(MWidgetController* controller);

    /*!
     * Destroys the lock screen view.
     */
    virtual ~LockScreenView();

protected:
    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

    //! Controller for the view
    MWidgetController *controller;

    //! Layout for the view
    QGraphicsLinearLayout *layout;

    //! Lock screen header widget
    LockScreenHeader *lockScreenHeader;

private slots:
    //! Updates the style name of the controller based on whether there are custom pixmaps in use
    void updateStyleName();

private:
    //! Data for the landscape pixmap
    LockScreenBackgroundPixmap landscapePixmap;

    //! Data for the portrait pixmap
    LockScreenBackgroundPixmap portraitPixmap;

    Q_DISABLE_COPY(LockScreenView)

#ifdef UNIT_TEST
    friend class Ut_LockScreenView;
#endif
};

#endif // LOCKSCREENVIEW_H
