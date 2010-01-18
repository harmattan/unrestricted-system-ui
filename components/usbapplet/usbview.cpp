/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiButtonGroup>
#include <DuiGConfItem>
#include <DuiLocale>

#include <QDebug>

#include "usbview.h"

UsbView::UsbView (QGraphicsWidget *parent)
    : DcpWidget (parent)
{
    m_GConfItem = new DuiGConfItem (USB_GCONF_KEY);

    QString         val = m_GConfItem->value ().toString ();

    // Lets use USB_AUTO when gconf key isn't set...
    m_current_mode = USB_AUTO;

    for (int i = 0; i <= USB_AUTO ; i++)
        if (val == usb_modes_str[i]) {
            m_current_mode = (usb_modes) i;
            break;
        }

    initWidget ();
}

UsbView::~UsbView ()
{
    delete m_GConfItem;
}

void 
UsbView::initWidget (void)
{
    DuiLayout               *mainLayout;
    DuiLinearLayoutPolicy   *policy;
    DuiButtonGroup          *button_group;

// Creating the main layout and policy

    mainLayout = new DuiLayout (this);
    policy = new DuiLinearLayoutPolicy (mainLayout, Qt::Vertical);
    mainLayout->setPolicy (policy);

    
// Creating & adding the info-label

    //% "You can select a default USB mode here:"
    m_info_label = new DuiLabel (qtTrId ("qtn_usb_info_label"));
    m_info_label->setObjectName ("info_label");
    m_info_label->setAlignment (Qt::AlignCenter);

    policy->addItem (m_info_label);

// Creating, filling and adding the mode-list
    button_group = new DuiButtonGroup ();

    //% "Ovi Suite"
    m_buttons[USB_OVI_SUITE] = new DuiButton (qtTrId ("qtn_usb_ovi_suite"));
    //% "Mass Storage"
    m_buttons[USB_MASS_STORAGE] = new DuiButton (qtTrId ("qtn_usb_mass_storage"));
    //% "Do nothing"
    m_buttons[USB_NOOP] = new DuiButton (qtTrId ("qtn_usb_do_nothing"));
    //% "Always ask"
    m_buttons[USB_AUTO] = new DuiButton (qtTrId ("qtn_usb_auto"));

    for (int i = 0; i <= USB_AUTO; i++) {
        m_buttons[i]->setCheckable (true);
        policy->addItem (m_buttons[i]);
        button_group->addButton (m_buttons[i], i);
    }

    connect (button_group, SIGNAL (buttonClicked (int)), this, SLOT (selectionChanged(int)));

    m_buttons[m_current_mode]->setChecked (true);
}

void
UsbView::selectionChanged (
        int id)
{
    if (id == (int)m_current_mode)
        return;

    m_current_mode = (usb_modes) id;
    m_GConfItem->set (QVariant (usb_modes_str[m_current_mode]));

    emit settingsChanged (id);
}

void
UsbView::retranslateUi ()
{
    m_info_label->setText (qtTrId ("qtn_usb_info_label"));

    m_buttons[USB_OVI_SUITE]->setText (qtTrId ("qtn_usb_ovi_suite"));
    m_buttons[USB_MASS_STORAGE]->setText (qtTrId ("qtn_usb_mass_storage"));
    m_buttons[USB_NOOP]->setText (qtTrId ("qtn_usb_do_nothing"));
    m_buttons[USB_AUTO]->setText (qtTrId ("qtn_usb_auto"));
}

