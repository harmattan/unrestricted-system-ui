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

#ifndef NGFADAPTER_H_
#define NGFADAPTER_H_

#include <QObject>

#include <dbus/dbus.h>

#ifdef HAVE_LIBNGF
#include <libngf/client.h>
#endif

/*!
 * A class that provides a simple QT'ified API to the plain C API provided
 * by the \c libngf0.
 *
 * This adapter class will request the libngf daemon to play a non-graphical
 * feedback when instructed to do so. Please note that this adapter will not
 * perform any error checking if the feedback was played or not as the
 * notification framework, the sole user of this class, really can not do
 * anything sensible with the error.
 */
class NGFAdapter : public QObject
{
    Q_OBJECT

public:

    /*!
     * Creates the adapter to the non-graphical feedback framework client
     */
    NGFAdapter();

    /*!
     * Destructs the adapter
     */
    ~NGFAdapter();

    /*!
     * Requests play from the non-graphical feedback daemon. It is not always
     * guaranteed that the tune will play.
     *
     * NOTE: The \c id will be converted to a \c const char* via QString::toUtf8().constData()
     *
     * \param id The id to play
     * \return eventId indicating the identifier of play request
     */
    uint play(const QString &id);

    /*!
     * Requests a stop from the non-graphical feedback daemon. If the play has already finished
     * this may not do anything.
     *
     * \param eventId identifier given by play
     */
    void stop(uint eventId);

    /*!
     * This method can be used to check if the adapter has managed to get to
     * create the low-level client and can play feedbacks
     *
     * \return true if this adapter can be used correctly, false otherwise.
     */
    bool isValid();


private:
#ifdef HAVE_LIBNGF
    //! The low-level ngf client
    NgfClient* client;

    //! The DBUS low-level connection
    DBusConnection* connection;
#endif
};

#endif /* NGFADAPTER_H_ */
