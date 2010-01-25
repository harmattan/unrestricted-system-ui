#ifndef USBBUSINESSLOGIC_H
#define USBBUSINESSLOGIC_H

#include "../usbapplet/usbmodes.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QDBusInterface>
#include <DuiGConfItem>

class UsbBusinessLogic : public QObject
{
    Q_OBJECT

    public:
        enum UsbCableType {
            CABLE_PERIPHERAL = 1,
            CABLE_HOST,
            CABLE_NONE
        };


        UsbBusinessLogic (QObject *parent = 0);
        ~UsbBusinessLogic ();

        void setMode (enum usb_modes new_mode);
        enum UsbCableType getCableType ();
        enum usb_modes getMode ();

    signals:
        void UsbCableEvent (enum UsbCableType cable);
        void PopUpDialog ();

    private:
        DuiGConfItem    *setting; 
        QDBusInterface  *hal;
}

#endif

