/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LOCKSCREENBUSINESSLOGICADAPTOR_H
#define LOCKSCREENBUSINESSLOGICADAPTOR_H

#include "lockscreenbusinesslogic.h"

#include <QDBusAbstractAdaptor>

/*!
 * Implements the DBus API interface for the crceen locking mechanism.
 */
class LockScreenBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.LockScreen")

public:
    LockScreenBusinessLogicAdaptor (
            QObject *                obj, 
            LockScreenBusinessLogic *delegate);
    //virtual ~LockScreenBusinessLogicAdaptor();

public slots:
    Q_NOREPLY void SetMissedEvents (
            int emails, 
            int messages, 
            int calls, 
            int im);

signals:
    void delegateSetMissedEvents (int, int, int, int);
};

#endif 
