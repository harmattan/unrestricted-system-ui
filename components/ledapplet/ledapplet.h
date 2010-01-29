/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LEDAPPLET_H
#define LEDAPPLET_H

#include "dcpappletif.h"

#include <QObject>

class LedDBusInterface;

class LedApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES (DcpAppletIf)

public:
    LedApplet ();
    ~LedApplet ();
    
    virtual void init();
    virtual DcpWidget* constructWidget(int widgetId);
    virtual QString title() const;
    virtual QVector<DuiAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    LedDBusInterface   *m_LedDBusInterface;
};

#endif
