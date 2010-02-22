/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>

class UsbUi;
class SystemUIGConf;
class BatteryBusinessLogic;
class LedBusinessLogic;
class ShutdownBusinessLogic;

class Sysuid : public QObject
{
    Q_OBJECT

public:
    Sysuid ();
    virtual ~Sysuid ();
    static Sysuid* sysuid ();
    static QString dbusService ();
    static QString dbusPath ();
    bool running_in_actdead_mode ();

public slots:
    void retranslate ();

private:
    SystemUIGConf           *m_SystemUIGConf;
    BatteryBusinessLogic    *m_BatteryLogic;
    LedBusinessLogic        *m_LedLogic;
    ShutdownBusinessLogic   *m_ShutdownLogic;
    UsbUi                   *m_UsbUi;
    static Sysuid           *m_Sysuid;
};

#endif // SYSUID_H
