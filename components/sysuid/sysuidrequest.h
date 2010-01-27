/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef SYSUIDREQUEST_H
#define SYSUIDREQUEST_H
#include <QObject>

class QString;
class LockScreenBusinessLogic;

/*!
 * This class creates a connection 
 */
class SysUidRequest : public QObject
{
    Q_OBJECT

public:
    SysUidRequest ();

private:
    QString dbusService ();
    QString dbusPath ();

    LockScreenBusinessLogic *m_LockScreenLogic;
};

#endif
