#include "usbui.h"
#include "usbbusinesslogic.h"

UsbUi::UsbUi (QObject *parent) : QObject (parent)
{
    logic = new UsbBusinessLogic (this);

    QObject::connect (logic, SIGNAL (PopUpDialog ()),
                      this, SLOT (ShowDialog ()));

    QObject::connect (logic, SIGNAL (UsbCableEvent (enum UsbCableType)),
                      this, SLOT (UsbEvent (enum UsbCableType)));
}

UsbUi::~UsbUi ()
{
    // TODO
}

// Showing the mode selection dialog
void
UsbUi::ShowDialog ()
{
    // TODO
}

// Showing notification on connection/disconnection
void
UsbUi::UsbEvent (enum UsbCableType cable)
{
    Q_UNUSED(cable)

    // TODO
}

