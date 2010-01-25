#ifndef USBUI_H
#define USBUI_H

#include <QObject>
#include "usbbusinesslogic.h"

class UsbUi : public QObject
{
    Q_OBJECT

    public:
        UsbUi (QObject *parent = 0);
        ~UsbUi ();

    public slots:
        void ShowDialog ();
        void UsbEvent (UsbCableType cable);

    private:
        UsbBusinessLogic    *logic;
};

#endif
