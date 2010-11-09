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
#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include <MWidgetView>
#include <MLocale>
#include <QScopedPointer>
#include "clockmodel.h"
#include "clockstyle.h"

class Clock;
class MLabel;

/*!
 * A view class for the Clock.
 */
class ClockView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(ClockModel, ClockStyle)

public:
    /*!
     * Constructs a new view for Clock. This view shows the clock's
     * time as a digital number.
     */
    ClockView(Clock *controller);

protected:
    //! \reimp
    virtual void applyStyle();
    void setupModel();
    //! \reimp_end

    //! A label to be used to render the clock time
    MLabel *label;

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    /*!
     * Gets the latest locale configuration and applies that to the display.
     */
    void applyCurrentLocale();

    /*!
     * Updates the visuals of this view
     */
    void updateLabel();

private:

    //! The previous label
    QString previousLabel;

    //! A pointer to the locale object
    QScopedPointer<MLocale> locale;

#ifdef UNIT_TEST
    friend class Ut_ClockView;
#endif
};

#endif
