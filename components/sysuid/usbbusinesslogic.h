#ifndef USBBUSINESSLOGIC_H
#define USBBUSINESSLOGIC_H

#include "../usbapplet/usbmodes.h"
#include <QObject>
#include <QString>
#include <DuiGConfItem>

class QDBusPendingCallWatcher;
class QDBusInterface;
class QDBusMessage;

enum UsbCableType {
    CABLE_PERIPHERAL = 1,
    CABLE_HOST,
    CABLE_NONE
};

class UsbBusinessLogic : public QObject
{
    Q_OBJECT

    public:
        UsbBusinessLogic (QObject *parent = 0);
        ~UsbBusinessLogic ();

        void setMode (enum usb_modes new_mode);
        enum UsbCableType getCableType ();
        enum usb_modes getMode ();

    signals:
        void UsbCableEvent (enum UsbCableType cable);
        void PopUpDialog ();

    private slots:
        void usb_prop_changed (const QDBusMessage &msg);
        void query_finished (QDBusPendingCallWatcher *call);

    private:
        DuiGConfItem    *setting; 
        QDBusInterface  *hal;

        void init_device (QString &udi);
};

#endif

