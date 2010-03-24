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

#ifndef CLOCK_H
#define CLOCK_H

#include <DuiWidgetController>
#include <QTimer>
#include <qmtime.h>
#include "clockmodel.h"

typedef Maemo::QmTimeWhatChanged QmTimeWhatChanged;

/*!
 * A widget for showing the current time.
 */
class Clock : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(Clock)

public:
    /*!
     * Constructs a new Clock.
     */
    explicit Clock(QGraphicsItem *parent = NULL);

private slots:
    /*!
     * \brief Updates the current time to the model and configures the timer for the next update.
     */
    void updateModelAndSetupTimer();

    /*!
     * \brief Updates the 24 hour mode if the settings have changed
     */
    void updateSettings(QmTimeWhatChanged whatChanged);

private:
    //! \reimp
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    //! \reimp_end

    //! Update timer
    QTimer timer;

    //! QmTime object to get the time format is
    Maemo::QmTime qmTime;
};

#endif
