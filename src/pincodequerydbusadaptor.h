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

    Q_CLASSINFO("D-Bus Introspection", ""
        "  <interface name=\"com.nokia.systemui.pin.PinCodeQuery\" >\n"
        "    <method name=\"ChangePinCode\" />\n"
        "    <signal name=\"PinQueryEnabled\" >\n"
        "      <arg type=\"s\" name=\"queryState\" />\n"
        "    </signal>\n"
        "    <method name=\"ChangePinCode\" >\n"
        "      <arg type=\"b\" name=\"enable\" />\n"
        "    </method>\n"
        "    <method name=\"LaunchPinQuery\" >\n"
        "      <arg type=\"s\" name=\"pinType\" />\n"
        "    </method>\n"
        "    <signal name=\"PinQueryDone\" >\n"
        "      <arg type=\"b\" name=\"queryOK\" />\n"
        "    </signal>\n"
        "  </interface>\n"
        "")

public:
    PinCodeQueryDBusAdaptor(PinCodeQueryBusinessLogic* logic);
    virtual ~PinCodeQueryDBusAdaptor();

    static QString dbusServiceName() {
      return QString("com.nokia.systemui.pin");
    }
    static QString dbusObjectName() {
      return QString("/com/nokia/systemui/pin");
    }
    static QString dbusInterfaceName() {
      return QString("com.nokia.systemui.pin.PinCodeQuery");
    }

    //! Response to \sa PinQueryState(bool enabled)
    void pinQueryStateCompletedResponse(SIMSecurity::PINQuery state, SIMError error);

    //! Response to \sa EnablePinQueryRequested(bool enabled)
    void pinQueryEnabledResponse(SIMSecurity::PINQuery queryState);

        //! Response to \sa LaunchPinQueryRequested(SIMSecurity::PINType pinType)
    void pinQueryDoneResponse(bool queryOk);

signals:
    /*!
     * Emits response to \sa PinQueryState(const QString &pinType).
     * Signal contains information if PIN query is enabled in device.
     *
     * \param state State of PIN query. \sa SIMSecurity::PINQuery
     * \param error Error occured during state query. \sa SIMError
     **/
    void PinQueryStateCompleted(SIMSecurity::PINQuery state);

    /*!
     * Emits response to \sa enablePinQuery(bool enable).
     * Signal contains information if PIN query is enabled in device.
     *
     * \param queryState State of PIN query. \sa SIMSecurity::PINQuery
     **/
    void PinQueryEnabled(SIMSecurity::PINQuery queryState);

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
    void PinQueryDone(bool queryOk);

public slots:
    /*!
     * Launches PIN query to change PIN.
     * Method doesn't launch the query if SIM status is not SIM::Ok
     **/
    Q_NOREPLY void ChangePinCode();

    /*!
     * Queries the current PIN state.
     * \sa PinQueryStateCompleted(SIMSecurity::PINQuery state, SIMError error)
     *
     * \param pinType Type of the pin to ask from user: PIN or PIN2.
     *      Allowed values are defined in \sa SIMSecurity::PINType.
     *      Parameter is not used.
     **/
    Q_NOREPLY void PinQueryState(const QString &pinType);

    /*!
     * Enables or disables PIN query.
     * Requests user to enter old PIN code and asks system to enable/disable PIN query.
     * \sa pinQueryEnabled(SIMSecurity::PINQuery queryState)
     *
     * \param enable Specifies the operation: \e true enable \e false disable.
     * dbus-send --session --print-reply --dest=com.nokia.systemui.pin /com/nokia/systemui/pin com.nokia.systemui.pin.PinCodeQuery.enablePinQuery
     **/
    Q_NOREPLY void EnablePinQuery(bool enable);

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
     * dbus-send --system --print-reply --dest=com.nokia.systemui.pin /com/nokia/systemui/pin com.nokia.systemui.pin.PinCodeQuery.LaunchPinQuery string:"PIN"
     **/
    bool LaunchPinQuery(const QString &pinType);

private:
    PinCodeQueryBusinessLogic* logic;

};

#endif // PINCODEQUERYDBUSADAPTOR_H

