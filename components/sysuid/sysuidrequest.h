/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef SYSUIDREQUEST_H
#define SYSUIDREQUEST_H
#include <QObject>

//class QString;
class LockScreenBusinessLogic;
//class EventHandler;
class QDBusConnection;

/*!
 * This class creates a connection between the system bus and the methods
 * implemented in the LockScreenBusinessLogicAdaptor where the methods
 * implemented for the MCE handling the locking of the touch screen.
 */
class SysUidRequest : public QObject
{
    Q_OBJECT

public:
    SysUidRequest ();

private:
    QString dbusService ();
    QString dbusPath ();
    void dbusError (QDBusConnection &connection, bool abortProgram = true);

    LockScreenBusinessLogic *m_LockScreenLogic;
};

#endif
