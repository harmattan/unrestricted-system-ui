/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "QDBusInterface"
#include "lockscreenbusinesslogicadaptor.h"

#define DEBUG
#include "../debug.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor (
        QObject                 *obj, 
        LockScreenBusinessLogic *logic) :
    QDBusAbstractAdaptor (obj)
{
    connect (this, SIGNAL(delegateSetMissedEvents(int, int, int, int)),
            logic, SLOT(updateMissedEventAmounts(int, int, int, int)));

    connect (logic, SIGNAL(unlockConfirmed()),
            this, SLOT(unlockConfirmed ()));
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
        const QString  &service,
        const QString  &path,
        const QString  &interface,
        const QString  &method,
        uint            mode,
        bool            silent,
        bool            flicker)
{
#if 1
    SYS_DEBUG (
"\n*** service   = '%s'"
"\n*** path      = '%s'"
"\n*** interface = '%s'"
"\n*** method    = '%s'"
"\n*** u         = %u"
"\n*** silent    = %s"
"\n*** flicker   = %s",
            SYS_STR (service),
            SYS_STR (path),
            SYS_STR (interface),
            SYS_STR (method),
            mode,
            silent ? "true" : "false",
            flicker ? "true" : "false");
#endif
    m_MCECallbackService = service;
    m_MCECallbackPath = path;
    m_MCECallbackInterface = interface;
    m_MCECallbackMethod = method;

    switch (mode) {
        case TkLockModeNone:
            SYS_DEBUG ("### TkLockModeNone");
            break;
        
        case TkLockModeEnable:
            SYS_DEBUG ("### TkLockModeEnable");
            break;

        case TkLockModeHelp:
            SYS_DEBUG ("### TkLockModeHelp");
            break;

        case TkLockModeSelect:
            SYS_DEBUG ("### TkLockModeSelect");
            break;

        case TkLockModeOneInput:
            SYS_DEBUG ("### TkLockModeOneInput");
            break;

        case TkLockEnableVisual:
            SYS_DEBUG ("### TkLockEnableVisual");
            break;

        default:
            SYS_WARNING ("### Unknown mode");
            break;
    }

    return (int) TkLockReplyOk;
}

/*
 * Here the MCE reports back that the lock screen should be closed. We actually
 * closed it already, so we do nothing, but we need this method otherwise MCE
 * will lock the screen again.
 *
 * FIXME: Maybe we should hide the screen locker here? 
 */
int
LockScreenBusinessLogicAdaptor::tklock_close (
        bool   b)
{
    SYS_DEBUG ("*** b = %s", b ? "true" : "false");

    return (int) TkLockReplyOk;
}

void 
LockScreenBusinessLogicAdaptor::unlockConfirmed ()
{
    QDBusInterface *dbusIf;
    SYS_DEBUG ("");

    dbusIf = new QDBusInterface (
            m_MCECallbackService, //"com.nokia.mce", 
            m_MCECallbackPath, //"/com/nokia/mce/request",
            m_MCECallbackInterface, //"com.nokia.mce.request",
            QDBusConnection::systemBus ());

    dbusIf->call (QDBus::NoBlock, 
            m_MCECallbackMethod, //QString ("tklock_callback"), 
            (int) TkLockReplyOk);

    delete dbusIf;
}

