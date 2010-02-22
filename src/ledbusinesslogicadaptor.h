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
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.leds")

public:
    LedBusinessLogicAdaptor (
            QObject          *parent, 
            LedBusinessLogic *ledLogic);

public slots:
    Q_NOREPLY void setLedsEnabled (bool enabled);
    Q_NOREPLY void setIlluminationLedEnabled (bool enabled);
    Q_NOREPLY void setEventsLedEnabled (bool enabled);
    bool LedsEnabled ();
    bool IlluminationLedEnabled ();
    bool EventsLedEnabled ();

signals: 
    void ledsStateChanged (bool);
    void illuminationLedStateChanged (bool);
    void eventsLedStateChanged (bool);

public:
    LedBusinessLogic   *m_LedLogic;
};

#endif
