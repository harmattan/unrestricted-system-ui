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

#ifndef LOWMEMORYNOTIFIER_H_
#define LOWMEMORYNOTIFIER_H_

#include <QObject>

namespace MEMNOTIFY {
    class MemoryNotification;
}

class MMessageBox;

/*!
 * Low memory notifier shows a message box when the memory is low.
 */
class LowMemoryNotifier : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a low memory notifier.
     */
    LowMemoryNotifier(QObject *parent = NULL);

    /*!
     * Destroys the low memory notifier.
     */
    virtual ~LowMemoryNotifier();

private slots:
    /*!
     * Handles the low memory notification by showing a message box if the memory is low.
     *
     * \param name the name of the signal
     * \param state \c true if the memory is low, \c false otherwise
     */
    void handleLowMemoryNotification(const QString &name, const bool state);

    /*!
     * Sets the title and text of the message box using the current translations.
     */
    void retranslateUi();

private:
    //! Low memory notifier
    MEMNOTIFY::MemoryNotification &memoryNotification;

    //! Message box for the notification
    MMessageBox *messageBox;

#ifdef UNIT_TEST
    friend class Ut_LowMemoryNotifier;
#endif
};

#endif /* LOWMEMORYNOTIFIER_H_ */
