#ifndef USBHAL_H
#define USBHAL_H

#include <QString>

#include <hal/libhal.h>

class UsbHal
{
public:
    enum State {
        InitFailed = -1,
        Invalid = 0,
        CableDetached,
        PeripheralWait,
        Host
    };

    UsbHal();
    ~UsbHal();

    /*!
      \brief Returns the USB cable state or InitFailed if HAL initialization failed
      */
    State getCableState();

    /*!
      \brief Returns the latest error message
      */
    const QString& lastError() const {
        return errorMsg;
    };

private:
    State initUdi();

    DBusConnection* conn;
    DBusError       error;
    LibHalContext*  ctx;
    char*           udi;
    State           state;
    QString         errorMsg;
};

#endif // USBHAL_H
