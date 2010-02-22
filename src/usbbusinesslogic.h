#ifndef USBBUSINESSLOGIC_H
#define USBBUSINESSLOGIC_H

#include <QObject>
#include <QString>
#include <DuiGConfItem>
#include <QDBusPendingCallWatcher>
#include <QDBusInterface>
#include <QDBusMessage>

class QProcess;

#include "../usbapplet/usbmodes.h"

class UsbBusinessLogic : public QObject
{
    Q_OBJECT

    public:
        UsbBusinessLogic (QObject *parent = 0);
        ~UsbBusinessLogic ();

        void setMode (usb_modes new_mode);
        usb_modes getModeSetting ();
        bool isActive ();
        bool isConnected ();

        void emitShowDialog ();
        // Its for testing:
        void emitConnected (bool connected);

    signals:
        void ShowDialog ();
        void Active (bool active);
        void Connected (bool connected);

    private slots:
        void usb_prop_changed (const QDBusMessage &msg);
        void query_finished (QDBusPendingCallWatcher *call);

    private:
        void init_device (QString &udi);
        int  getCableType ();

        DuiGConfItem    *m_setting; 
        QDBusInterface  *m_hal;
        bool             m_active;
        bool             m_connected;
        QProcess        *m_process;

};

#endif

