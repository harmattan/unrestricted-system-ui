/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "lockscreenbusinesslogicadaptor.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor(
    QObject *obj, LockScreenBusinessLogic *logic)
        : QDBusAbstractAdaptor(obj)
{
    connect (this, SIGNAL(delegateSetMissedEvents(int, int, int, int)),
            logic, SLOT(updateMissedEventAmounts(int, int, int, int)));
}

#if 0
LockScreenBusinessLogicAdaptor::~LockScreenBusinessLogicAdaptor()
{
}
#endif

void 
LockScreenBusinessLogicAdaptor::SetMissedEvents (
        int emails, 
        int messages, 
        int calls, 
        int im)
{
    emit delegateSetMissedEvents (emails, messages, calls, im);
}
