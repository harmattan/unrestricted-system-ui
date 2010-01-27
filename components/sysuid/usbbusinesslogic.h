#ifndef USBBUSINESSLOGIC_H
#define USBBUSINESSLOGIC_H

#include <QObject>
#include <QString>
#include <DuiGConfItem>
#include <QDBusPendingCallWatcher>
#include <QDBusInterface>
#include <QDBusMessage>

#include "../usbapplet/usbmodes.h"

typedef enum {
    CABLE_PERIPHERAL = 1,
    CABLE_HOST,
    CABLE_NONE
} UsbCableType;

class UsbBusinessLogic : public QObject
{
    Q_OBJECT

    public:
        UsbBusinessLogic (QObject *parent = 0);
        ~UsbBusinessLogic ();

        void setMode (usb_modes new_mode);
        UsbCableType getCableType ();
        usb_modes getMode ();

    signals:
        void UsbCableEvent (UsbCableType cable);
        void PopUpDialog ();

    private slots:
        void usb_prop_changed (const QDBusMessage &msg);
        void query_finished (QDBusPendingCallWatcher *call);

    private:
        void init_device (QString &udi);

        DuiGConfItem    *setting; 
        QDBusInterface  *hal;

};

#endif

