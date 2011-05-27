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
#ifndef USBUI_H
#define USBUI_H

#include <QObject>
#include <QPointer>
#include <MDialog>
#include <MNotification>

#ifdef HAVE_QMSYSTEM
#include <qmusbmode.h>
#endif

namespace MeeGo {
    class QmLocks;
}

class MGConfItem;

/*!
 * The USB UI takes care of USB related user interfaces.
 */
class UsbUi : public QObject {
    Q_OBJECT

public:
    /*!
     * Constructs an USB UI.
     */
    UsbUi(QObject *parent = NULL);

    /*!
     * Destroys the USB UI.
     */
    virtual ~UsbUi();

    enum NotificationCategory {
        Mode = 0,
        Error
    };

signals:
    //! Signaled when the USB mode dialog is shown.
    void dialogShown();

private slots:
#ifdef HAVE_QMSYSTEM
    /*!
     * Shows the USB dialog/banners based on the current USB mode.
     */
    void applyCurrentUSBMode();

    /*!
     * Shows the USB dialog/banners based on the given USB mode.
     *
     * \param mode the USB mode to show UI elements for
     */
    void applyUSBMode(MeeGo::QmUSBMode::Mode mode);

    /*!
     * Sets the USB mode to the requested mode (if any).
     */
    void setRequestedUSBMode();
#endif
    /*!
     * Sets the USB mode to Ovi Suite.
     */
    void setOviSuiteMode();

    /*!
     * Sets the USB mode to mass storage.
     */
    void setMassStorageMode();

    /*!
     * Sets the USB mode to SDK.
     */
    void setSDKMode();

    /*!
     * Shows the mode selection dialog.
     */
    void showDialog();

    /*!
     * Shows an error string.
     *
     * \param error the translation ID of the error to show
     */
    void showError(const QString &error);

private:
#ifdef HAVE_QMSYSTEM
    /*!
     * Shows a notification.
     *
     * \param category the category of the notification (Mode/Error)
     * \param mode the USB mode for the notification
     */
    void showNotification(NotificationCategory category, MeeGo::QmUSBMode::Mode mode);
#endif

    //! Hides the currently active notification from the given category
    void hideNotification(NotificationCategory category);

    //! Hides the mode selection dialog if it exists, accepting or rejecting it
    void hideDialog(bool accept);

#ifdef HAVE_QMSYSTEM
    MeeGo::QmUSBMode *usbMode;
    MeeGo::QmUSBMode::Mode requestedUSBMode;
    MeeGo::QmLocks *locks;
#endif

    //! Currently active notifications
    QMap<NotificationCategory, MNotification> notifications;

    //! Mode selection dialog
    QPointer<MDialog> dialog;

    //! GConf item which tracks if developer mode is enabled
    MGConfItem *developerMode;

#ifdef UNIT_TEST
    friend class Ut_UsbUi;
#endif
};

#endif
