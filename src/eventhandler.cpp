#include "eventhandler.h"

EventHandler::EventHandler()
{
    keys = new QmKeys();
    //alarm = new QmAlarm();

    //Monitor hardware key presses
    connect(keys, SIGNAL(keyPressed(QmKeys::Key key, QmKeys::HowPressed how)),
            this, SLOT(keyPressed(QmKeys::Key key, QmKeys::HowPressed how)));

    //Monitor alarms
    //connect(alarm, SIGNAL(ring(qmAlarmCookie)), this, SLOT(alarmClockEvent(qmAlarmCookie)));
}

EventHandler::~EventHandler()
{
    delete keys;
    keys = NULL;
    //delete alarm;
    //alarm = NULL;
}

void EventHandler::keyPressed(QmKeys::Keys key, QmKeys::HowPressed how)
{
    switch(key) {
        case QmKeys::PowerKey:
            switch(how) {
                case QmKeys::ShortPress:
                    emit shortPowerKeyPressOccured();
                break;
                case QmKeys::LongPress:
                break;
            }
        break;
        case QmKeys::HomeKey:
        break;
        case QmKeys::CameraShutterKey:
        break;
        default:
        break;
    }
}

void EventHandler::alarmClockEvent(qmAlarmCookie)
{
}
