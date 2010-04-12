/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef USBMODES_H
#define USBMODES_H

#define USB_GCONF_KEY   "/M/System/UsbMode"

typedef enum {
    USB_OVI_SUITE = 0,
    USB_MASS_STORAGE,
    USB_NOOP,
    USB_AUTO
} usb_modes;

extern const char * usb_modes_str[];

#endif
