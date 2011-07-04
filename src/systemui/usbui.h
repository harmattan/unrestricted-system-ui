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

#ifdef HAVE_QMSYSTEM
#include <qmusbmode.h>
#endif

namespace MeeGo {
    class QmLocks;
}

class MGConfItem;
class QGraphicsLinearLayout;
class MLabel;
class MBasicListItem;

/*!
 * The USB UI takes care of USB related user interfaces.
 */
class UsbUi : public MDialog {
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

signals:
    //! Signaled when the USB mode dialog is shown.
    void dialogShown();

private slots:
    //! \reimp
    virtual void retranslateUi();
    //! \reimp_end

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
     * Shows an error string matching the given error code, if any.
     *
     * \param errorCode the error code of the error to be shown
     */
    void showError(const QString &errorCode);

    //! Shows or hides the SDK item from the dialog based on whether developer mode is enabled or not
    void updateSDKItemVisibility();

private:
#ifdef HAVE_QMSYSTEM
    /*!
     * Shows a notification.
     *
     * \param mode the USB mode for the notification
     */
    void showNotification(MeeGo::QmUSBMode::Mode mode);
#endif

    //! Hides the mode selection dialog if it exists, accepting or rejecting it
    void hideDialog(bool acceptDialog);

#ifdef HAVE_QMSYSTEM
    MeeGo::QmUSBMode *usbMode;
    MeeGo::QmUSBMode::Mode requestedUSBMode;
    MeeGo::QmLocks *locks;
#endif

    //! MeeGo::QmUSBMode::error() error code to translation ID mapping
    static QMap<QString, QString> errorCodeToTranslationID;

    //! GConf item which tracks if developer mode is enabled
    MGConfItem *developerMode;

    //! Layout for the dialog
    QGraphicsLinearLayout *layout;

    //! Current state: charging label
    MLabel *chargingLabel;

    //! Mass storage mode list item
    MBasicListItem *massStorageItem;

    //! Ovi Suite mode list item
    MBasicListItem *oviSuiteItem;

    //! SDK mode list item
    MBasicListItem *sdkItem;

#ifdef UNIT_TEST
    friend class Ut_UsbUi;
#endif
};

#endif
