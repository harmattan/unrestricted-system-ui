/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef DISPLAYAPPLET_H
#define DISPLAYAPPLET_H

#include <DcpAppletIf>
#include <DcpWidget>
#include <DcpBrief>
#include <QObject>

class DuiAction;
class DisplayWidget;

class DisplayApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    virtual void init();
    virtual DcpWidget* constructWidget(int widgetId);
    virtual DcpWidget* pageMain();
    virtual QString title() const;
    virtual QVector<DuiAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    QPointer<DisplayWidget> main;

};
#endif // DISPLAYAPPLET_H
