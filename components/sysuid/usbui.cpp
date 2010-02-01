#include "usbui.h"
#include "usbbusinesslogic.h"

#include <QGraphicsLinearLayout>
#include <DuiWidget>
#include <DuiNotification>
#include <DuiSceneWindow>
#include <DuiButton>
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

    QObject::connect (m_logic, SIGNAL (Connected (bool)),
                      this, SLOT (UsbEvent (bool)));

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
void
UsbUi::ShowDialog ()
{
    SYS_DEBUG ("");
    DuiButton  *button;
    DuiWidget  *hbox;

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

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;

    //% "Ovi Suite"
    button = new DuiButton (qtTrId ("qtn_usb_ovi_suite"), m_dialog);

    layout->addItem (button);
    layout->setStretchFactor (button, 1);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (OviSuiteSelected ()));

    //% "Mass Storage"
    button = new DuiButton (qtTrId ("qtn_usb_mass_storage"), m_dialog);

    layout->addItem (button);
    layout->setStretchFactor (button, 1);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (MassStorageSelected ()));

    hbox = new DuiWidget;
    hbox->setLayout (layout);

    m_dialog->setCentralWidget (hbox);

    m_dialog->setButtonBoxVisible (false);
    m_dialog->appear ();
}

void
UsbUi::OviSuiteSelected ()
{
    SYS_DEBUG ("");

    m_logic->setMode (USB_OVI_SUITE);

    m_dialog->disappear ();
}

void
UsbUi::MassStorageSelected ()
{
    SYS_DEBUG ("");

    m_logic->setMode (USB_MASS_STORAGE);

    m_dialog->disappear ();
}

// Showing notification on connection/disconnection
void
UsbUi::UsbEvent (bool connected)
{
    SYS_DEBUG ("");

    // remove previous one
    if (m_notification)
    {
        m_notification->remove ();
        delete m_notification;
        m_notification = 0;
    }

    if (connected == false)
    {
        //% "Usb disconnected"
        m_notification = new DuiNotification ("",
                                              "",
                                              qtTrId ("qtn_usb_disconnected"),
                                              "icon-m-usb");

        // Hide the mode-selection dialog
        if (m_dialog)
            m_dialog->disappear ();

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
        new DuiNotification ("",
                             "",
                             qtTrId ("qtn_usb_connected_%1").arg (*mode_text),
                             "icon-m-usb");
}

// for dbus adaptor
UsbBusinessLogic *
UsbUi::getLogic ()
{
    return m_logic;
}

