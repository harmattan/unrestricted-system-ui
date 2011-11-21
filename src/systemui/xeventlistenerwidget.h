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

#ifndef XEVENTLISTENERWIDGET_H_
#define XEVENTLISTENERWIDGET_H_

#include <QWidget>
#include <X11/Xlib.h>

/*!
 * A QWidget which listens to the X events of the given window and relays them
 * to the XEventListener.
 */
class XEventListenerWidget : public QWidget {
    Q_OBJECT

public:
    /*!
     * Creates an XEventListenerWidget.
     *
     * \param parent the parent widget for the widget
     * \param id the ID of the X window for the window to be represented by this widget
     */
    XEventListenerWidget(QWidget *parent, Window id);

    /*!
     * Destroys the XEventListenerWidget.
     */
    ~XEventListenerWidget();

protected:
    //! \reimp
    virtual bool x11Event(XEvent *event);
    //! \reimp_end

#ifdef UNIT_TEST
    friend class Ut_XEventListenerWidget;
#endif
};

#endif /* XEVENTLISTENERWIDGET_H_ */
