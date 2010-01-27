#ifndef USBMODES_H
#define USBMODES_H

#define USB_GCONF_KEY   "/Dui/System/UsbMode"

typedef enum {
    USB_OVI_SUITE = 0,
    USB_MASS_STORAGE,
    USB_NOOP,
    USB_AUTO
} usb_modes;

extern const char * usb_modes_str[];

#endif
