#include "usbbusinesslogic.h"

#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

#include <QString>

#define DEBUG
#define WARNING
#include "../debug.h"

#define DEFAULT_USB_CABLE_UDI \
    "/org/freedesktop/Hal/devices/usb_device_1d6b_2_musb_hdrc"

UsbBusinessLogic::UsbBusinessLogic (QObject *parent) :
    QObject (parent),
    setting (0),
    hal (0)
{
    setting = new DuiGConfItem (USB_GCONF_KEY);

    QDBusInterface HalManager ("org.freedesktop.Hal",
                               "/org/freedesktop/Hal/Manager",
                               "org.freedesktop.Hal.Manager",
                               QDBusConnection::systemBus (),
                               this);

    // Do the usb-cable-button UDI finding async:
    QDBusPendingCall async =
        HalManager.asyncCall ("FindDeviceStringMatch",
                              QVariant ("button.type"),
                              QVariant ("usb.cable"));

    QDBusPendingCallWatcher *watcher =
        new QDBusPendingCallWatcher (async, this);

    QObject::connect (watcher, SIGNAL (finished (QDBusPendingCallWatcher *)),
                      this, SLOT (query_finished (QDBusPendingCallWatcher *)));
}

UsbBusinessLogic::~UsbBusinessLogic ()
{
}

// This function will not set the gconf-key value,
// that one is only modifiable by usb-applet,
// this function will only set the actual mode
void
UsbBusinessLogic::setMode (usb_modes new_mode)
{
    // FIXME: ke-recv needed to do these things ...
    switch (new_mode)
    {
        case USB_OVI_SUITE:  
            // TODO: ask ke-recv to switch device to
            //       pc-suite mode
            break;
        case USB_MASS_STORAGE:
            // TODO: ask ke-recv to switch device to
            //       usb-mass-storage mode
            break;
        case USB_NOOP:
        default:
            /* Do nothing, ie. Charging only ... */
            break;
    }
}

usb_modes
UsbBusinessLogic::getMode ()
{
    QString   val = setting->value ().toString (); 
    usb_modes current_mode = USB_AUTO;

    for (int i = 0; i <= USB_AUTO; i++)
        if (val == usb_modes_str[i])
        {
            current_mode = (usb_modes) i;
            break;
        }

    return current_mode;
}

UsbCableType
UsbBusinessLogic::getCableType ()
{
    QDBusMessage reply = hal->call("GetProperty", "usb_device.mode");

    if (reply.type() != QDBusMessage::ReplyMessage)
    {
        return CABLE_NONE;
    }

    if (reply.errorName() != "org.freedesktop.Hal.NoSuchProperty")
    {
        QString str = reply.arguments().at(0).toString ();
        if (str.contains ("peripheral"))
            return CABLE_PERIPHERAL;
        else if (str.contains ("host"))
            return CABLE_HOST;
    }

    return CABLE_NONE;
}

void
UsbBusinessLogic::query_finished (QDBusPendingCallWatcher *call)
{
    // Init UDI with the default one (as a fallback value...)
    QString udi (DEFAULT_USB_CABLE_UDI);

    QDBusPendingReply<QStringList> reply = *call;
    if (reply.isError ())
    {
        SYS_WARNING ("Determining usb-cable-udi failed, "
                     "using the default one:" DEFAULT_USB_CABLE_UDI);
    }
    else
    {
        QStringList list = reply.value ();

        if (list.isEmpty () == false)
        { // The first UDI seems okey :-)
            udi = list.first ();
            SYS_DEBUG ("found UDI: %s", udi.toLatin1 ().constData ());
        }
        else
            SYS_WARNING ("Determining usb-cable-udi failed, "
                         "using the default one:" DEFAULT_USB_CABLE_UDI);
    }

    // Set up the connection with the found device (or with the default one)
    init_device (udi);
}

void
UsbBusinessLogic::init_device (QString &udi)
{
    hal = new QDBusInterface ("org.freedesktop.Hal",
                              udi,
                              "org.freedesktop.Hal.Device",
                              QDBusConnection::systemBus (),
                              this);

    hal->connection ().connect (
        "org.freedesktop.Hal", udi, "org.freedesktop.Hal.Device",
        "PropertyModified", // <- SLOT
        this, SLOT (usb_prop_changed (const QDBusMessage &)));
}

void
UsbBusinessLogic::usb_prop_changed (const QDBusMessage &msg)
{
    UsbCableType current = getCableType ();

    Q_UNUSED(msg)

#ifdef DEBUG
    switch (current)
    {
        case CABLE_PERIPHERAL:
            SYS_DEBUG ("%s: peripheral-mode", __func__);
            break;
        case CABLE_HOST:
            SYS_DEBUG ("%s: host-mode", __func__);
            break;
        case CABLE_NONE:
        default:
            SYS_DEBUG ("%s: idle", __func__);
            break;
    }
#endif

    // INFO: i can show usb-cable-(dis)connected info msg...
    emit (UsbCableEvent (current));

    if (current != CABLE_PERIPHERAL)
        return; // no-op

    // Get the Usb applet setting for GConf...
    usb_modes mode = getMode ();

    if (mode == USB_AUTO)
    { 
        // Cable just plugged in, and the current mode is auto,
        // lets show the mode-selection dialog:
        emit PopUpDialog ();
    }
    else
    {
        // Switch the device to the selected mode,
        // (without questions :-P)
        setMode (mode);
    }
}

