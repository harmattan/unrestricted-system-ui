#include "usbui.h"
#include "usbbusinesslogic.h"

#include <DuiNotification>
#include <DuiSceneWindow>
#include <DuiButton>
#include <DuiLabel>
#include <DuiDialog>
#include <DuiLocale>
#include <QTimer>

#define DEBUG
#define WARNING
#include "../debug.h"

UsbUi::UsbUi (QObject *parent) : QObject (parent),
    m_logic (0),
    m_notification (0),
    m_dialog (0)
{
    m_logic = new UsbBusinessLogic (this);

    QObject::connect (m_logic, SIGNAL (UsbCableEvent (UsbCableType)),
                      this, SLOT (UsbEvent (UsbCableType)));

    // It is for UsbBusinessLogicAdaptor:
    QObject::connect (m_logic, SIGNAL (ShowDialog ()),
                      this, SLOT (ShowDialog ()));
}

UsbUi::~UsbUi ()
{
    if (m_dialog)
    {
        m_dialog->disappearNow ();
        delete m_dialog;
        m_dialog = 0;
    }
}

// Showing the mode selection dialog
// (when mode is usb-auto / "ask on connection")
// TODO: test this, how does it looks?
void
UsbUi::ShowDialog ()
{
    SYS_DEBUG ("");
    DuiButtonModel  *button;

    if (m_dialog)
    {
        m_dialog->appear ();
        m_dialog->setFocus ();
        return;
    }

    m_dialog = new DuiDialog;

    //% "Usb connected"
    m_dialog->setTitle (qtTrId ("qtn_usb_connected_title"));
    m_dialog->setWindowModal (true);

    //% "Select USB mode:"
    DuiLabel (qtTrId ("qtn_usb_select_usb_mode"),
              m_dialog->centralWidget ()).setAlignment (Qt::AlignCenter);

    //% "Ovi Suite"
    button = m_dialog->addButton (qtTrId ("qtn_usb_ovi_suite"));
    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (OviSuiteSelected ()));

    //% "Mass Storage"
    button = m_dialog->addButton (qtTrId ("qtn_usb_mass_storage"));
    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (MassStorageSelected ()));

    m_dialog->appear ();
}

void
UsbUi::OviSuiteSelected ()
{
    SYS_DEBUG ("");
    m_logic->setMode (USB_OVI_SUITE);
    // XXX: do we need this?
    m_dialog->disappear ();
}

void
UsbUi::MassStorageSelected ()
{
    SYS_DEBUG ("");
    m_logic->setMode (USB_MASS_STORAGE);
    // XXX: do we need this?
    m_dialog->disappear ();
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
    // FIXME: add some nice icons to the notificitations

    if (cable == CABLE_NONE)
    {
        //% "Usb-cable disconnected"
        m_notification = new DuiNotification ("", "", qtTrId ("qtn_usb_disconnected"));

        // Hide the mode-selection dialog
        if (m_dialog)
            m_dialog->disappear ();

        return;
    }
    else if (cable == CABLE_HOST)
    {
        // Not implemented ...
        return;
    }

    usb_modes   config    = m_logic->getModeSetting ();
    QString    *mode_text = 0;

    // Activate the desired usb mode
    if (config != USB_AUTO)
        m_logic->setMode (config);

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
        case USB_AUTO:
            ShowDialog ();
            return;
    } 

    //% "<b>Usb connected</b><br />Selected mode: <b>%1</b>"
    m_notification =
        new DuiNotification ("", "", qtTrId ("qtn_usb_connected_%1").arg (*mode_text));
}

// for dbus adaptor
UsbBusinessLogic *
UsbUi::getLogic ()
{
    return m_logic;
}

