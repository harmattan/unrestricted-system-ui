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
#ifndef DATE_H_
#define DATE_H_

#include <MStylableWidget>
#include <QTimer>
#include "datestyle.h"

#ifdef HAVE_QMSYSTEM
#include <qmtime.h>
#endif

class MLabel;
class MLocale;

/*!
 * Shows the date
 */
class Date : public MStylableWidget
{
    Q_OBJECT

public:
    /*!
     * Constructor
     */
    Date(QGraphicsItem *parent = 0);

    /*!
     * Destructor
     */
    virtual ~Date();

protected:
    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

private slots:
    /*!
     * Updates the labels with current time and schedules the next update
     */
    void updateDate();

#ifdef HAVE_QMSYSTEM
    /*!
     * Updates the labels if the time was changed in settings
     */
    void updateSettings(MeeGo::QmTime::WhatChanged whatChanged);
#endif

private:
    //! Label for the date
    MLabel *label;

    //! System locale
    MLocale *locale;

    //! Update timer
    QTimer timer;

    //! Date format string
    QString dateFormat;

#ifdef HAVE_QMSYSTEM
    //! QmTime object for listening date changes
    MeeGo::QmTime qmTime;
#endif

    M_STYLABLE_WIDGET(DateStyle)

#ifdef UNIT_TEST
    friend class Ut_Date;
#endif
};

#endif
