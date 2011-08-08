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

#ifndef STATUSINDICATOR_H
#define STATUSINDICATOR_H

#include <MWidgetController>
#include <QDBusConnection>
#include "statusindicatormodel.h"
#include <QTimer>

#ifdef HAVE_QMSYSTEM
#include <qmdevicemode.h>
#endif

class ApplicationContext;
class ContextItem;
class MGConfItem;

/*!
 * StatusIndicator is a widget for displaying either image or text based status
 * indicators in the status indicator area of the home screen.
 */
class StatusIndicator : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(StatusIndicator)

    Q_PROPERTY(QVariant value READ value WRITE setValue)

public:
    /*!
     * Constructs a StatusIndicator.
     *
     * \param parent parent MWidget
     */
    explicit StatusIndicator(QGraphicsItem *parent = NULL);

    /*!
     * \brief Destructor
     */
    virtual ~StatusIndicator();

    //! \reimp
    // Reimplemented here as public so that the view can call it
    virtual void updateGeometry();
    //! \reimp_end

signals:
    //! Emitted when a subscription to the context items has been made
    void subscriptionMade();

protected:
    /*!
     * Calls setStyleName(name) and update() for convenience.
     *
     * \param name the style name to set
     */
    void setStyleNameAndUpdate(const QString &name = QString());

    /*!
     * Sets the value of the indicator. This will be used by the dynamic
     * indicators to change their appearance based on some value. The type of the value
     * is determined by the indicator type. For indicators that display
     * an image from a list of images, the value is a float 0 to 1 which
     * is used to determine the used image. For indicators that display text,
     * the value is the string to display.
     */
    void setValue(QVariant v);

    /*!
     * Gets the value of the indicator. \see setValue
     */
    QVariant value() const;

    //! Updates the animation status of the views
    void updateAnimationStatus();

    //! Whether or not the view should animate if possible
    bool animateIfPossible;

    /*!
     * Returns a context item with the specified key newly allocated
     * from the specified application context.
     *
     * \param context Context framework application context for the context
     * item to be allocated from.
     * \param key Unique intifier of the context item in the application
     * context that is to be monitored.
     * \return New ContextItem object tracking the particular context item
     * in the system. The created ContextItem is owned by the StatusIdicator
     * class and shall not be deleted by the caller.
     */
    ContextItem *createContextItem(ApplicationContext& context, const QString& key);

private:
    //! \reimp
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    //! \reimp_end

    /*!
     * Enables or disables model updates.
     *
     * \param modelUpdatesEnabled \c true if the model should be updated, \c false otherwise
     */
    void setModelUpdatesEnabled(bool modelUpdatesEnabled);

    //! Whether the model should be updated or not
    bool modelUpdatesEnabled;

    //! The current value of the status indicator
    QVariant currentValue;

    //! A list of context items the indicator is tracking
    QList<ContextItem*> contextItems;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicator;
#endif
};

/*!
 * A status indicator for showing the phone network signal strength
 */
class PhoneNetworkSignalStrengthStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(PhoneNetworkSignalStrengthStatusIndicator)

public:
    /*!
     * Constructs a PhoneNetworkSignalStrengthStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget
     */
    explicit PhoneNetworkSignalStrengthStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~PhoneNetworkSignalStrengthStatusIndicator();

signals:
    void networkAvailabilityChanged(bool available);

private slots:
    void signalStrengthChanged();
    void setNetworkStatus();

private:
    ContextItem *signalStrength;
    ContextItem *systemOfflineMode;
    ContextItem *cellularRegistrationStatus;

    bool networkAvailable;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicator;
#endif
};

/*!
 * A status indicator for showing the phone network type and
 * the internet connection status.
 * Displays either a WiFi or a packet data connection symbol. The
 * symbol is animated while the connection is being established.
 * If a packet data connection is active (data is being transfered),
 * a "packet data connection active" symbol is shown.
 */

class PhoneNetworkTypeStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(PhoneNetworkTypeStatusIndicator);

public:
    /*!
     * Constructor
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget
     */
    explicit PhoneNetworkTypeStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~PhoneNetworkTypeStatusIndicator();

public slots:
    void setNetworkAvailability(bool availability);

private slots:
    void setNetworkType();

private:

    ContextItem *cellularDataTechnology;
    ContextItem *connectionType;
    ContextItem *connectionState;
    ContextItem *packetData;
    ContextItem *wlanEnabled;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicator;
#endif

};

/*!
 * A status indicator for showing the battery charge level or
 * battery charging animation.
 */
class BatteryStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(BatteryStatusIndicator)

public:
    /*!
     * Constructs a BatteryStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget
     */
    explicit BatteryStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~BatteryStatusIndicator();

private slots:
    void batteryLevelChanged();
    void batteryChargingChanged();

private:
    ContextItem *batteryLevel;
    ContextItem *batteryCharging;
    ContextItem *batterySaveMode;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicator;
#endif
};

/*!
 * A status indicator for showing whether a clock alarm has been set.
 */
class AlarmStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(AlarmStatusIndicator)

public:
    /*!
     * Constructs a AlarmStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget
     */
    explicit AlarmStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~AlarmStatusIndicator();

private slots:
    void alarmChanged();

private:
    ContextItem *alarm;
};

/*!
 * A status indicator for showing whether the bluetooth connection
 * is activated.
 */
class BluetoothStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(BluetoothStatusIndicator)

public:
    /*!
     * Constructs a BluetoothStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget
     */
    explicit BluetoothStatusIndicator(ApplicationContext &context,
                                      QGraphicsItem *parent = NULL);

    virtual ~BluetoothStatusIndicator();

private slots:
    void bluetoothChanged();

private:
    ContextItem *bluetoothEnabled;
    ContextItem *bluetoothConnected;
};

/*!
 * A status indicator for showing the global presence setting.
 */
class PresenceStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(PresenceStatusIndicator)

public:
    /*!
     * Constructs a PresenceStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget
     */
    explicit PresenceStatusIndicator(ApplicationContext &context,
                                      QGraphicsItem *parent = NULL);

    virtual ~PresenceStatusIndicator();

private slots:
    void presenceChanged();

private:
    ContextItem *presence;
};


/*!
 * A status indicator for showing the used phone network name.
 * The phone network name is cropped to 13 characters.
 * Sets the value to the phone network name.
 */
class PhoneNetworkStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(PhoneNetworkStatusIndicator)

public:
    /*!
     * Constructs a PhoneNetworkStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget. If parent is NULL, the creating client must take the responsibility to delete the indicator.
     */
    explicit PhoneNetworkStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~PhoneNetworkStatusIndicator();

private slots:
    void phoneNetworkChanged();
    void showVisitorNetworkName();

private:
    QString homeNetwork() const;
    QString visitorNetwork() const;
    QString localizedNetwork() const;

    ContextItem *networkName;
    ContextItem *extendedNetworkName;
    ContextItem *cellularServiceStatus;
    ContextItem *cellularRegistrationStatus;
    MGConfItem *displayLimitedServiceState;
    QTimer networkChangeShowVisitorTimer;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicator;
#endif
};

/*!
 * A status indicator for showing the current input method.
 * Sets the value to the related icon ID.
 */
class InputMethodStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(InputMethodStatusIndicator)

public:
    /*!
     * Constructs a InputMethodStatusIndicator.
     *
     * \param parent parent MWidget
     */
    explicit InputMethodStatusIndicator(QGraphicsItem *parent = NULL);

    virtual ~InputMethodStatusIndicator();

    void setIconID(const QString &iconID);
};

/*!
 * A status indicator for showing the call status.
 * Displays incoming, ongoing or ongoing + muted call status.
 * Sets the value to the related icon ID.
 */
class CallStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(CallStatusIndicator)

public:
    explicit CallStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~CallStatusIndicator();

private slots:
    void callOrMutedChanged();

private:
    ContextItem *call;
    ContextItem *muted;
};

/*!
 * A status indicator for showing the current profile.
 * Displayed only when the profile is silent.
 */
class ProfileStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(ProfileStatusIndicator)

public:
    explicit ProfileStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~ProfileStatusIndicator();

private slots:
    void profileChanged();

private:
    ContextItem *profile;
};

/*!
* A status indicator for showing the GPS connection state
 */
class GPSStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(GPSStatusIndicator)

public:
    /*!
     * Constructs a GPSStatusIndicator.
     *
     * \param context the application context to get GPS status information from
     * \param parent parent MWidget. If parent is NULL, the creating client must take the responsibility to delete the indicator.
     */
    explicit GPSStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~GPSStatusIndicator();

private slots:
    void gpsStateChanged();

private:
    ContextItem *gpsState;
};

/*!
 * A status indicator for showing call forwarding is activated.
 */
class CallForwardingStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(CallForwardingStatusIndicator)

public:
    /*!
     * Constructs a CallForwardingStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget
     */
    explicit CallForwardingStatusIndicator(ApplicationContext &context, QGraphicsItem *parent = NULL);

    virtual ~CallForwardingStatusIndicator();

private slots:
    void callForwardingChanged();

private:
    ContextItem *callForwarding;
};

/*!
 * A status indicator for showing the status of transfer.
 * Shows ongoing transfer and error in transfer.
 */
class TransferStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(TransferStatusIndicator)

public:
    /*!
     * Constructs a TransferStatusIndicator. Starts listening changes in transfer's state
     * from Transfer UI dbus interface.
     *
     * \param parent parent QGraphicsItem. If parent is NULL, the creating client must take the responsibility to delete the indicator.
     */
    explicit TransferStatusIndicator(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the TransferStatusIndicator.
     */
    virtual ~TransferStatusIndicator();


private slots:
    /*!
     * Sets object's object name to resemble the transfer state.
     * States are: idle, live, fail.
     *
     * \param state of the transfer operation.
     */
    void transferStateChanged(const QString &state);

private:
    //! Connection to dbus session bus
    QDBusConnection connectionSessionBus;

    //! dbus path that is listened
    static const QString TRANSFER_UI_DBUS_PATH;
    //! dbus interface that is listened
    static const QString TRANSFER_UI_DBUS_INTERFACE;
    //! dbus signal call that delivers transfer state
    static const QString TRANSFER_UI_DBUS_SIGNAL;
    //! State that is received when transfer state becomes idle
    static const QString TRANSFER_UI_STATE_IDLE;
    //! State notifying that there is at least one active transfer and no errors
    static const QString TRANSFER_UI_STATE_LIVE;
    //! State notifying that there is at least one failed transfer
    static const QString TRANSFER_UI_STATE_FAIL;
    //! State notifying that there is only pending and paused transfers
    static const QString TRANSFER_UI_STATE_PENDING;
    //! Suffix that is added to style name when state is fail
    static const QString TRANSFER_UI_SUFFIX_FAIL;
    //! Suffix that is added to style name when state is live
    static const QString TRANSFER_UI_SUFFIX_LIVE;
    //! Suffix that is added to style name when state is pending
    static const QString TRANSFER_UI_SUFFIX_PENDING;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicator;
#endif

};

/*!
 * A status indicator for showing the notification status.
 * Sets the value to the related icon ID.
 */
class NotificationStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(NotificationStatusIndicator)

public:
    /*!
     * Constructs a NotificationStatusIndicator.
     *
     * \param parent parent QGraphicsItem
     */
    explicit NotificationStatusIndicator(QGraphicsItem *parent = NULL);

    virtual ~NotificationStatusIndicator();

public slots:
    void setIconID(const QString &iconID);
};

#endif // STATUSINDICATOR_H
