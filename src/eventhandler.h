#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "lockscreenbusinesslogic.h"
#include <qmsystem/qmalarm.h>
#include <qmsystem/qmkeys.h>

using namespace Maemo;

class EventHandler : public QObject
{
    Q_OBJECT

public:
    EventHandler();
    virtual ~EventHandler();

signals:
    void shortPowerKeyPressOccured();

private slots:
    void keyPressed(QmKeys::Keys key, QmKeys::HowPressed how);
    void alarmClockEvent(qmAlarmCookie);

private: //attributes
    QmKeys *keys;
    //QmAlarm *alarm;


};

#endif // EVENTHANDLER_H
