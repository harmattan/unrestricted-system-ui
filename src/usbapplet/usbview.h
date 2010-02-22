/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef USBVIEW_H
#define USBVIEW_H

#include "usbmodes.h"
#include <QString>
#include <DuiGConfItem>
#include <DuiLabel>
#include <DuiButton>
#include <DcpWidget>

class UsbView : public DcpWidget 
{
    Q_OBJECT

public:
    UsbView (QGraphicsWidget *parent = 0);
	~UsbView ();

signals:
    void settingsChanged      (int currentmode);

private slots:
    void selectionChanged     (int id);

private:
    void initWidget (void);
    void retranslateUi ();

    DuiGConfItem   *m_GConfItem;
    usb_modes       m_current_mode;
    DuiButton      *m_buttons[USB_AUTO+1];
    DuiLabel       *m_info_label;
};

#endif

