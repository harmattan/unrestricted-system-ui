/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "lockscreenbusinesslogicadaptor.h"

#define DEBUG
#include "../debug.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor (
    QObject *obj, 
    LockScreenBusinessLogic *logic) :
    QDBusAbstractAdaptor(obj)
{
    connect (this, SIGNAL(delegateSetMissedEvents(int, int, int, int)),
            logic, SLOT(updateMissedEventAmounts(int, int, int, int)));
}


void 
LockScreenBusinessLogicAdaptor::SetMissedEvents (
        int emails, 
        int messages, 
        int calls, 
        int im)
{
    SYS_DEBUG (
"\n*** emails    = %d"
"\n*** messages  = %d"
"\n*** calls     = %d"
"\n*** im        = %d",
    emails, messages, calls, im);

    emit delegateSetMissedEvents (emails, messages, calls, im);
}



int 
LockScreenBusinessLogicAdaptor::tklock_open (
        const QString  &a,
        const QString  &b,
        const QString  &c,
        const QString  &d,
        uint            mode,
        bool            silent,
        bool            flicker)
{
    SYS_DEBUG (
"\n*** a         = '%s'"
"\n*** b         = '%s'"
"\n*** c         = '%s'"
"\n*** d         = '%s'"
"\n*** u         = %u"
"\n*** silent    = %s"
"\n*** flicker   = %s",
            SYS_STR (a),
            SYS_STR (b),
            SYS_STR (c),
            SYS_STR (d),
            mode,
            silent ? "true" : "false",
            flicker ? "true" : "false");

    switch (mode) {
        case TkLockModeNone:
            SYS_DEBUG ("*** TkLockModeNone");
            break;
        
        case TkLockModeEnable:
            SYS_DEBUG ("*** TkLockModeEnable");
            break;

        case TkLockModeHelp:
            SYS_DEBUG ("*** TkLockModeHelp");
            break;

        case TkLockModeSelect:
            SYS_DEBUG ("*** TkLockModeSelect");
            break;

        case TkLockModeOneInput:
            SYS_DEBUG ("*** TkLockModeOneInput");
            break;

        case TkLockEnableVisual:
            SYS_DEBUG ("*** TkLockEnableVisual");
            break;

        default:
            SYS_WARNING ("*** Unknown mode");
            break;
    }

    return (int) TkLockReplyOk;
}

/*
 * This is not working. 
 */
void
LockScreenBusinessLogicAdaptor::tklock_close ()
{
    SYS_DEBUG ("");
}
