/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef USBAPPLET_H
#define USBAPPLET_H

#include <DcpAppletIf>
#include <QObject>

class DcpWidget;
class DuiAction;
class UsbBrief;

class UsbApplet : public QObject, public DcpAppletIf 
{
	Q_OBJECT
	Q_INTERFACES(DcpAppletIf)

public:
    virtual void                init();
	virtual DcpWidget*          constructWidget(int widgetId);
    virtual QString             title() const;
    virtual QVector<DuiAction*> viewMenuItems();
    virtual DcpBrief*           constructBrief(int partId);

private:
    UsbBrief  *m_Brief;
    
};

#endif

