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
    #if 1
    // This is what used in MCE.
    Q_CLASSINFO("D-Bus Interface", "com.nokia.system_ui.request")
    #else
    // This is what others want.
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.LockScreen")
    #endif

public:
    LockScreenBusinessLogicAdaptor (
            QObject *                obj, 
            LockScreenBusinessLogic *delegate);

    /*
     * I have no information about these, copyed from osso-systemui-tklock.
     */
    enum {
        TkLockReplyFailed = 0,
        TkLockReplyOk     = 1
    } TklockReply;

    enum {
        TkLockModeNone,
        TkLockModeEnable,
        TkLockModeHelp,
        TkLockModeSelect,
        TkLockModeOneInput,
        TkLockEnableVisual
    } TkLockMode;
    
public slots:
    Q_NOREPLY void SetMissedEvents (
            int emails, 
            int messages, 
            int calls, 
            int im);

    int tklock_open (
            const QString  &a,
            const QString  &b,
            const QString  &c,
            const QString  &d,
            uint       u,
            bool       b1,
            bool       b2);

    /*
     * This is not working. 
     */
    int tklock_close (bool b);

private slots:
    void unlockConfirmed ();

signals:
    void delegateSetMissedEvents (int, int, int, int);

private:
    QString  m_MCECallbackService;
    QString  m_MCECallbackPath;
    QString  m_MCECallbackInterface;
    QString  m_MCECallbackMethod;
};

#endif 
