#ifndef USBBUSINESSLOGICADAPTOR_H
#define USBBUSINESSLOGICADAPTOR_H

#include "usbbusinesslogic.h"

#include <QDBusAbstractAdaptor>

// Implements the DBus API interface for the USB UI
class UsbBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.usb")

public:
    UsbBusinessLogicAdaptor (
            QObject             *parent,
            UsbBusinessLogic    *usb);

signals:
    // Its emitted when some USB mode activated/deactivated
    // (Ovi Suite mode, Mass Storage mode)
    // This can be used eg. for the status-area usb icon
    void Active (bool active);
    void Connected (bool connected);

public slots:
    // Its for testing: 
    Q_NOREPLY void testUsbConnection (bool connected);
    // Its for eg.: statusindicator menu plugin:
    Q_NOREPLY void ShowModeSelectionDialog ();

    bool isActive (); 
    bool isConnected ();

private:
    UsbBusinessLogic   *m_usb;
};

#endif 
