#include "usbui.h"
#include "usbbusinesslogic.h"

#include <DuiNotification>
#include <DuiSceneWindow>
#include <DuiLocale>
#include <QTimer>

#define DEBUG
#define WARNING
#include "../debug.h"

UsbUi::UsbUi (QObject *parent) : QObject (parent)
{
    SYS_DEBUG ("UsbUi init");

    m_logic = new UsbBusinessLogic (this);

    QObject::connect (m_logic, SIGNAL (PopUpDialog ()),
                      this, SLOT (ShowDialog ()));

    QObject::connect (m_logic, SIGNAL (UsbCableEvent (UsbCableType)),
                      this, SLOT (UsbEvent (UsbCableType)));
}

UsbUi::~UsbUi ()
{
}

// Showing the mode selection dialog
void
UsbUi::ShowDialog ()
{
    // TODO
    SYS_WARNING ("Not implemented: Show USB mode selection dialog");
}

// Showing notification on connection/disconnection
void
UsbUi::UsbEvent (UsbCableType cable)
{
    SYS_DEBUG ("");

    // remove previous one
    if (m_notification)
    {
        m_notification->remove ();
        delete m_notification;
        m_notification = 0;
    }
    // TODO: add some nice icons to the notificitations

    if (cable == CABLE_NONE)
    {
        //% "Usb-cable disconnected"
        m_notification = new DuiNotification ("", "", qtTrId ("qtn_usb_disconnected"));

        return;
    }
    else if (cable == CABLE_HOST)
    {
        // Not implemented ...
        return;
    }

    usb_modes   config    = m_logic->getMode ();
    QString    *mode_text = 0;

    switch (config)
    {
        case USB_OVI_SUITE:
            //% "Ovi Suite"
            mode_text = new QString (qtTrId ("qtn_usb_ovi_suite"));
            break;
        case USB_MASS_STORAGE:
            //% "Mass Storage"
            mode_text = new QString (qtTrId ("qtn_usb_mass_storage"));
            break;
        case USB_NOOP:
            //% "Do nothing"
            mode_text = new QString (qtTrId ("qtn_usb_do_nothing"));
            break;
        default:
            // no-op, on setting USB_NOOP
            // and on USB_AUTO (a ShowDialog signal will come)
            return;
    } 

    //% "<b>Usb connected<br />Selected mode: <i>%1</i>"
    m_notification =
        new DuiNotification ("", "", qtTrId ("qtn_usb_connected_%1").arg (*mode_text));
}

