/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QTimer>

#include <qmsystem/qmalarm.h>
#include <qmsystem/qmkeys.h>

using namespace Maemo;

/*!
 * Handles hw key press and release events.
 *
 * FIXME: handling of the alarms has been commented out. I don't know why, so I
 * left the lines there.
 */
class EventHandler : public QObject
{
    Q_OBJECT

public:
    EventHandler (QObject* parent = 0);
    virtual ~EventHandler ();

signals:
    void shortPowerKeyPressOccured ();
    void longPowerKeyPressOccured (bool btnDown);
    void powerKeyDownOccured ();
    void powerKeyUpOccured ();

private slots:
    void keyPressed (QmKeys::Keys key, QmKeys::HowPressed how);
    //void alarmClockEvent (qmAlarmCookie);

    void hwBtnDown (int code);
    void hwBtnUp (int code);
    void pwrKeyHold ();


private: //attributes
    QmKeys       *m_Keys;
    //QmAlarm *alarm;
    QTimer       *m_PowerPressTimer;
    int           m_PwrCheckCount;

};

#endif // EVENTHANDLER_H
