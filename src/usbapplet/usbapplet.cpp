/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <QDebug>
#include <DuiAction>
#include <DuiTheme>
#include <DcpWidget>
#include <DuiLocale>

#include "usbbrief.h"
#include "usbview.h"
#include "usbapplet.h"

Q_EXPORT_PLUGIN2(usbapplet, UsbApplet)

const QString cssDir = "/usr/share/themes/base/dui/duicontrolpanel/style/";

void 
UsbApplet::init (void)
{
    DuiTheme::loadCSS (cssDir + "usbapplet.css");

    m_Brief = NULL;
}

DcpWidget *
UsbApplet::constructWidget (
        int widgetId)
{
    UsbView  *view;

    Q_UNUSED(widgetId);

    view = new UsbView ();

    connect (view,  SIGNAL (settingsChanged (int)), 
             m_Brief, SLOT (settingsChanged (int)));

    return view;
}

QString 
UsbApplet::title (void) const
{
    //% "USB"
    return qtTrId ("qtn_usb_title");
}

QVector<DuiAction*> 
UsbApplet::viewMenuItems (
        void)
{
    QVector<DuiAction*> vector;

    return vector;
}

DcpBrief * 
UsbApplet::constructBrief (
        int partId)
{
    Q_UNUSED(partId);
    
    m_Brief = new UsbBrief();
    
    return m_Brief;
}

