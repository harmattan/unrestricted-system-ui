/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LEDBUSINESSLOGICADAPTOR_H
#define LEDBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>

class LedBusinessLogic;

/*!
 * Provides a DBus interface to control the device leds.
 */
class LedBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.led")

public:
    LedBusinessLogicAdaptor (
            QObject          *parent, 
            LedBusinessLogic *ledLogic);

public:
    LedBusinessLogic   *m_LedLogic;
};

#endif
