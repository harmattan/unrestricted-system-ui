/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef USBBRIEF_H
#define USBBRIEF_H

#include <DuiLocale>
#include <DcpBrief>

class UsbBrief: public DcpBrief {
    Q_OBJECT

public:
    UsbBrief();
    virtual QString valueText() const;

public slots:
    void    settingsChanged (int currentmode);

private:
    void    retranslateUi();
    int     m_CurrentMode;
};


#endif
