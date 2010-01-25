#ifndef USBUI_H
#define USBUI_H

#include <QObject>
#include "usbbusinesslogic.h"

class UsbUi : public QObject
{

    public:
        UsbUi (QObject *parent = 0);
        ~UsbUi ();

    private slots:
        void ShowDialog ();
        void UsbEvent (enum UsbCableType cable);

    private:
        UsbBusinessLogic    *logic;
};

#endif
