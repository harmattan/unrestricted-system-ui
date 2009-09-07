#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QTimer>

#include <qmsystem/qmalarm.h>
#include <qmsystem/qmkeys.h>

//#include "lockscreenbusinesslogic.h"





using namespace Maemo;

class EventHandler : public QObject
{
    Q_OBJECT

public:
    EventHandler();
    virtual ~EventHandler();

signals:
    void shortPowerKeyPressOccured();
    void longPowerKeyPressOccured(bool btnDown);
private slots:
    void keyPressed(QmKeys::Keys key, QmKeys::HowPressed how);
    void alarmClockEvent(qmAlarmCookie);

    void hwBtnDown(int code);
    void hwBtnUp(int code);
    void pwrKeyHold();


private: //attributes
    QmKeys *keys;
    //QmAlarm *alarm;

    QTimer *powerPressTimer;
    int pwrCheckCount;

};

#endif // EVENTHANDLER_H
