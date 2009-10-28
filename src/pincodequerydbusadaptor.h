#ifndef PINCODEQUERYDBUSADAPTOR_H
#define PINCODEQUERYDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <SIM>

/**
  * DBus adaptor class for PIN code query
  */

using namespace Cellular;

class PinCodeQueryDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.PinCodeQuery")

public:
    PinCodeQueryDBusAdaptor();
    virtual ~PinCodeQueryDBusAdaptor();
    static QString dbusInterfaceName();

    //! Response to \sa enablePinQueryRequested(bool enabled)
    void pinQueryEnabledResponse(SIMSecurity::PINQuery queryState);

signals:
    //! Emitted to business logic to change PIN query
    void changePinCodeRequested();

    //! Emitted to business logic to enable/disable PIN query
    void enablePinQueryRequested(bool enabled);

    /*!
     * Emits response to \sa enablePinQuery(bool enable).
     * Signal contains information if PIN query is enabled in device.
     *
     * \param queryState State of PIN query. \sa SIMSecurity::PINQuery
     **/
    void pinQueryEnabled(SIMSecurity::PINQuery queryState);

    /*!
     * Informs PIN query status after query is launched.
     * PIN query sequence ends when first \sa launchPinQuery(SIMSecurity::PINType pinType)
     * is called and setting the PIN is successfully done, user has cancelled it or some error occured
     * during setting it.
     *
     * \param currentSimStatus Current status of the PIN.
     * \param queryOk Operation state: \e true, if PIN query is successfully executed, \e false if
     * either user has cancelled the query or error occured during operation.
     **/
    void pinQueryDone(SIM::SIMStatus currentSimStatus, bool queryOk);

public slots:
    /*!
     * Launches PIN query to change PIN.
     * Method doesn't launch the query if SIM status is not SIM::Ok
     **/
    Q_NOREPLY void changePinCode();

    /*!
     * Enables or disables PIN query.
     * Requests user to enter old PIN code and asks system to enable/disable PIN query.
     * \sa pinQueryEnabled(SIMSecurity::PINQuery queryState)
     *
     * \param enable Specifies the operation: \e true enable \e false disable.
     **/
    Q_NOREPLY void enablePinQuery(bool enable);

    /*!
     * Launches PIN query.
     *
     * \param pinType Type of the pin to ask from user: PIN or PIN2. Parameter is not used.
     * \return action. If query is launched returns \e true, \e false otherways.
     * SIM statuses when the query is not launched:
     * \li \c SIM::UnknownStatus
     * \li \c SIM::Ok
     * \li \c SIM::NoSIM
     * \li \c SIM::PermanentlyBlocked
     * \li \c SIM::NotReady
     * \li \c SIM::Rejected
     * \sa SIM
     **/
    bool launchPinQuery(SIMSecurity::PINType pinType);

};

#endif // PINCODEQUERYDBUSADAPTOR_H
