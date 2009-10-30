#ifndef PINCODEQUERYDBUSADAPTOR_H
#define PINCODEQUERYDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <SIM>

/**
  * DBus adaptor class for PIN code query
  */
class PinCodeQueryBusinessLogic;
using namespace Cellular;

class PinCodeQueryDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.pin.PinCodeQuery")

public:
    PinCodeQueryDBusAdaptor(PinCodeQueryBusinessLogic* logic);
    virtual ~PinCodeQueryDBusAdaptor();
    static QString dbusInterfaceName();

    //! Response to \sa enablePinQueryRequested(bool enabled)
    void pinQueryEnabledResponse(SIMSecurity::PINQuery queryState);

        //! Response to \sa launchPinQueryRequested(SIMSecurity::PINType pinType)
    void pinQueryDoneResponse(SIM::SIMStatus currentSimStatus, bool queryOk);

signals:
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
     * dbus-send --session --print-reply --dest=com.nokia.systemui / com.nokia.systemui.PinCodeQuery.enablePinQuery
     **/
    Q_NOREPLY void enablePinQuery(bool enable);

    /*!
     * Launches PIN query.
     *
     * \param pinType Type of the pin to ask from user: PIN or PIN2.
     *      Allowed values are defined in \sa SIMSecurity::PINType.
     *      Parameter is not used.
     * \return action. If query is launched returns \e true, \e false otherways.
     * SIM statuses when only the query is launched:
     * \li \c SIM::PINRequired
     * \li \c SIM::PUKRequired
     * \li \c SIM::SIMLockRejected
     * \sa SIM
     **/
    bool launchPinQuery(int pinType);

private:
    PinCodeQueryBusinessLogic* logic;

};

#endif // PINCODEQUERYDBUSADAPTOR_H
