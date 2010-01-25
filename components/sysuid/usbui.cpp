#include "usbui.h"
#include "usbbusinesslogic.h"

#include <DuiInfoBanner>
#include <DuiSceneWindow>
#include <DuiLocale>
#include <QTimer>

UsbUi::UsbUi (QObject *parent) : QObject (parent)
{
    logic = new UsbBusinessLogic (this);

    QObject::connect (logic, SIGNAL (PopUpDialog ()),
                      this, SLOT (ShowDialog ()));

    QObject::connect (logic, SIGNAL (UsbCableEvent (enum UsbCableType)),
                      this, SLOT (UsbEvent (enum UsbCableType)));
}

UsbUi::~UsbUi ()
{
    // TODO
}

// Showing the mode selection dialog
void
UsbUi::ShowDialog ()
{
}

// Showing notification on connection/disconnection
void
UsbUi::UsbEvent (enum UsbCableType cable)
{
    // TODO: add some nice icons to info-banners
    DuiInfoBanner   *infoBanner = 0;

    if (cable == CABLE_NONE)
    {
        infoBanner = new DuiInfoBanner (DuiInfoBanner::Information);

        //% "Usb-cable disconnected"
        infoBanner->setBodyText (qtTrId ("qtn_usb_disconnected"));
        infoBanner->appear (DuiSceneWindow::DestroyWhenDone);
        QTimer::singleShot (3000, infoBanner, SLOT (dissappear ()));

        return;
    }
    else if (cable == CABLE_HOST)
    {
        // Not implemented ...
        return;
    }

    usb_modes   config    = logic->getMode ();
    QString    *mode_text = 0;

    switch (config)
    {
        case USB_OVI_SUITE:
            //% "Ovi Suite"
            mode_text = new QString (qtTrId ("qtn_usb_ovi_suite"));
            break;
        case USB_MASS_STORAGE:
            //% "Mass Storage"
            mode_text = new QString (qtTrId ("qtn_usb_mass_storage"));
            break;
        case USB_NOOP:
            //% "Do nothing"
            mode_text = new QString (qtTrId ("qtn_usb_do_nothing"));
            break;
        default:
            // no-op, on setting USB_NOOP
            // and on USB_AUTO (a ShowDialog signal will come)
            return;
    } 

    infoBanner = new DuiInfoBanner (DuiInfoBanner::Information);
    //% "<b>Usb connected<br />Selected mode: <i>%1</i>"
    infoBanner->setBodyText (
        qtTrId ("qtn_usb_connected_mode").arg (*mode_text));
    infoBanner->appear (DuiSceneWindow::DestroyWhenDone);
    QTimer::singleShot (3000, infoBanner, SLOT (dissappear ()));
}

