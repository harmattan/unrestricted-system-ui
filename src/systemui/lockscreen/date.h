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

#include <MWidget>
#include <QTimer>

#ifdef HAVE_QMSYSTEM
#include <qmtime.h>
#endif

class MLabel;
class MLocale;

/*!
 * Shows the day of the week, month and day
 */
class Date : public MWidget
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

    /*!
     * Sets the format string of the date label
     * \param format the format string
     */
    void setFormat(const QString &format);

private slots:
    /*!
     * Updates the labels with current time and schedules the next update
     */
    void updateDate();

#ifdef HAVE_QMSYSTEM
    /*!
     * Updates the labels if the time was changed in settings
     */
    void updateSettings(Maemo::QmTimeWhatChanged whatChanged);
#endif

private:
    //! Label for the date
    MLabel *label;

    //! System locale
    MLocale *locale;

    //! Format string of the date label
    QString format;

    //! Update timer
    QTimer timer;

#ifdef HAVE_QMSYSTEM
    //! QmTime object for listening date changes
    Maemo::QmTime qmTime;
#endif

#ifdef UNIT_TEST
    friend class Ut_Date;
#endif
};

#endif
