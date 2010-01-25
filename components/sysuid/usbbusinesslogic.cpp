#include "usbbusinesslogic.h"
#include <QDBusConnection>
#include <QString>

#define USB_CABLE_UDI \
    "/org/freedesktop/Hal/devices/usb_device_1d6b_2_musb_hdrc"

UsbBusinessLogic::UsbBusinessLogic (QObject *parent) :
    QObject (parent),
    setting (0),
    hal (0)
{
    setting = new DuiGConfItem (USB_GCONF_KEY);
    hal = new QDBusInterface ("org.freedesktop.Hal",
                              USB_CABLE_UDI,
                              "org.freedesktop.Hal.Device",
                              QDBusConnection::systemBus (),
                              this);
/*                              
    hal->connection ().connect ("org.freedesktop.Hal",
                                USB_CABLE_UDI,
                                "org.freedesktop.Hal.Device",
                                "PropertyModified",
                                this
                                SIGNAL (propertyModified (int, const QList<ChangeDescription> &)));
 */
}

UsbBusinessLogic::~UsbBusinessLogic ()
{
    // TODO: drop hal connections
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

enum UsbCableType
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

