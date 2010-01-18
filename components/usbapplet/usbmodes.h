#ifndef USBMODES_H
#define USBMODES_H

#define USB_GCONF_KEY   "/Dui/System/UsbMode"

enum usb_modes {
    USB_OVI_SUITE = 0,
    USB_MASS_STORAGE,
    USB_NOOP,
    USB_AUTO
};

extern const char * usb_modes_str[];

#endif
