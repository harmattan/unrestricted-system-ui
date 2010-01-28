#include "usbbusinesslogicadaptor.h"

#define WARNING
#include "../debug.h"

UsbBusinessLogicAdaptor::UsbBusinessLogicAdaptor (
    QObject             *parent,
    UsbBusinessLogic    *usb) :
    QDBusAbstractAdaptor (parent),
    m_usb (usb)
{
    QObject::connect (m_usb, SIGNAL (Active (bool)),
                      this, SIGNAL (UsbModeChanged (bool)));
}

void
UsbBusinessLogicAdaptor::testUsbConnection (
    bool connected)
{
    if (connected)
        m_usb->emitUsbCableEvent (CABLE_PERIPHERAL);
    else
        m_usb->emitUsbCableEvent (CABLE_NONE);
}

void
UsbBusinessLogicAdaptor::ShowModeSelectionDialog ()
{
    if (m_usb->getCableType () != CABLE_PERIPHERAL)
    {
        SYS_WARNING (" called when usb peripheral cable isn't connected!");
    }
    m_usb->emitShowDialog (); 
}

// This function just proxy-ing the query
// to the businesslogic isActive method
bool
UsbBusinessLogicAdaptor::UsbMode ()
{
    return m_usb->isActive ();
}


