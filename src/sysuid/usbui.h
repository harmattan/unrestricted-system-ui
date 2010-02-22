#ifndef USBUI_H
#define USBUI_H

#include <QObject>
#include "usbbusinesslogic.h"

class DuiDialog;
class DuiNotification;

class UsbUi : public QObject
{
    Q_OBJECT

    public:
        UsbUi (QObject *parent = 0);
        ~UsbUi ();
        UsbBusinessLogic * getLogic ();

    private slots:
        void ShowDialog ();
        void UsbEvent (bool Connected);
        void OviSuiteSelected ();
        void MassStorageSelected ();

    private:
        UsbBusinessLogic    *m_logic;
        DuiNotification     *m_notification;
        DuiDialog           *m_dialog;

        void show_notification (int id);
};

#endif
