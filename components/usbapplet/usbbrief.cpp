/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <DuiLocale>
#include <QLocale>
#include <QVariant>
#include <QString>
#include <QDebug>

#include <DuiGConfItem>
#include "usbbrief.h"
#include "usbmodes.h"

UsbBrief::UsbBrief ()
{
    DuiGConfItem    gconf_item (USB_GCONF_KEY);
    QString         val = gconf_item.value ().toString ();

    m_CurrentMode = USB_AUTO;

    for (int i = 0; i <= USB_AUTO ; i++)
        if (val == usb_modes_str[i]) {
            m_CurrentMode = (usb_modes) i;
            break;
        }
}

void 
UsbBrief::settingsChanged (
        int currentmode)
{
    m_CurrentMode = currentmode;

    emit valuesChanged ();
}

void
UsbBrief::retranslateUi ()
{
    // The translated text (valueText) may change...
    emit valuesChanged (); 
}

QString
UsbBrief::valueText(void) const
{
    switch (m_CurrentMode)
    {
        case USB_OVI_SUITE:
            //% "Ovi Suite"
            return qtTrId ("qtn_usb_ovi_suite");
            break;
        case USB_MASS_STORAGE:
            //% "Mass Storage"
            return qtTrId ("qtn_usb_mass_storage");
            break;
        case USB_NOOP:
            //% "Do nothing"
            return qtTrId ("qtn_usb_do_nothing");
            break;
        case USB_AUTO:
        default:
            //% "Always ask"
            return qtTrId ("qtn_usb_auto");
            break;
    }
}

