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
                      this, SIGNAL (Active (bool)));
    QObject::connect (m_usb, SIGNAL (Connected (bool)),
                      this, SIGNAL (Connected (bool)));
}

void
UsbBusinessLogicAdaptor::testUsbConnection (
    bool connected)
{
    m_usb->emitConnected (connected);
    // And of course, UsbBusinessLogic should emit itself connected signal:
    emit Connected (connected);
}

void
UsbBusinessLogicAdaptor::ShowModeSelectionDialog ()
{
    if (m_usb->isConnected () == false)
    {
        SYS_WARNING (" called when usb peripheral cable isn't connected!");
    }
    m_usb->emitShowDialog (); 
}

// This function just proxying the query
// to the businesslogic isActive method
bool
UsbBusinessLogicAdaptor::isActive ()
{
    return m_usb->isActive ();
}

// This function just proxying the query
// to the businesslogic isConnected method
bool
UsbBusinessLogicAdaptor::isConnected ()
{
    return m_usb->isConnected ();
}

