#include "usbbusinesslogic.h"

#include <QProcess>
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

enum UsbCableType {
    CABLE_PERIPHERAL = 1,
    CABLE_HOST,
    CABLE_NONE
};

UsbBusinessLogic::UsbBusinessLogic (QObject *parent) :
    QObject (parent),
    m_setting (0),
    m_hal (0),
    m_active (false),
    m_connected (false)
{
    m_setting = new DuiGConfItem (USB_GCONF_KEY, this);

    m_process = new QProcess;

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
    delete m_setting;
    m_setting = 0;

    delete m_process;
    m_process = 0;
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
#ifdef __ARMEL__
            SYS_DEBUG ("Running: /usr/bin/pcsuite-enable.sh");
            // FIXME: ask ke-recv to switch device to
            //       pc-suite mode
            m_process->start ("/usr/bin/sudo /usr/bin/pcsuite-enable.sh");
#else
            SYS_DEBUG ("Ovi Suite mode activated (no-op in i386 target)");
#endif
            m_active = true;
            emit Active (true);
            break;
        case USB_MASS_STORAGE:
            SYS_DEBUG ("not-implemented: Mass Storage mode");
            // TODO: ask ke-recv to switch device to
            //       usb-mass-storage mode
            m_active = true;
            emit Active (true);
            break;
        case USB_NOOP:
        default:
            SYS_DEBUG ("doing nothing");
            m_active = false;
            /* Do nothing, ie. Charging only ... */
            break;
    }
}

usb_modes
UsbBusinessLogic::getModeSetting ()
{
    usb_modes current_mode = USB_AUTO;

    if (m_setting->value ().type() == QVariant::Invalid)
        return current_mode;

    QString   val = m_setting->value ().toString (); 

    for (int i = 0; i <= USB_AUTO; i++)
        if (val == usb_modes_str[i])
        {
            current_mode = (usb_modes) i;
            break;
        }

    return current_mode;
}

int
UsbBusinessLogic::getCableType ()
{
    QDBusMessage reply = m_hal->call("GetProperty", "usb_device.mode");

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
            SYS_DEBUG ("found UDI: %s", SYS_STR (udi));
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
    m_hal = new QDBusInterface (
                "org.freedesktop.Hal",
                udi,
                "org.freedesktop.Hal.Device",
                QDBusConnection::systemBus (),
                this);

    m_hal->connection ().connect (
        "org.freedesktop.Hal", udi, "org.freedesktop.Hal.Device",
        "PropertyModified", // <- SLOT
        this, SLOT (usb_prop_changed (const QDBusMessage &)));

    if (getCableType () == CABLE_PERIPHERAL)
    {
        m_connected = true;
//        emit Connected (m_connected);
    }
}

void
UsbBusinessLogic::usb_prop_changed (const QDBusMessage &msg)
{
    int current = getCableType ();

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

    if (current != CABLE_PERIPHERAL)
    { // peripheral cable disconnected
        if (m_active == true)
        {
            m_active = false;
            emit Active (false);
        }
        
        m_connected = false;
    }
    else
        m_connected = true;

    emit Connected (m_connected);
}

// Returns whether the device has an active usb connection
// (aka Ovi Suite or Mass Storage mode...)
bool
UsbBusinessLogic::isActive ()
{
    return m_active;
}

// Returns true when a peripheral usb cable connected to the device
bool
UsbBusinessLogic::isConnected ()
{
    return m_connected;
}

void
UsbBusinessLogic::emitConnected (bool connected)
{
    emit Connected (connected);
}

void
UsbBusinessLogic::emitShowDialog ()
{
    emit ShowDialog ();
}

