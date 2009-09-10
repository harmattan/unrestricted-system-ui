
#include <QDebug>

// last include, please
#include "eventhandler.h"

const int SHORT2LONG = 2000/*ms*/;


EventHandler::EventHandler()
{
    powerPressTimer = NULL;

    keys = new QmKeys();
    //alarm = new QmAlarm();

    //Monitor hardware key presses
    connect(keys, SIGNAL(keyPressed(QmKeys::Keys, QmKeys::HowPressed)),
            this, SLOT(keyPressed(QmKeys::Keys, QmKeys::HowPressed)));

    //Monitor alarms
    //connect(alarm, SIGNAL(ring(qmAlarmCookie)), this, SLOT(alarmClockEvent(qmAlarmCookie)));

    connect(keys, SIGNAL(keyDown(int)), this, SLOT(hwBtnDown(int)));
    connect(keys, SIGNAL(keyUp(int)), this, SLOT(hwBtnUp(int)));
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

void EventHandler::pwrKeyHold()
{
    QTimer *timer = static_cast<QTimer*>(this->sender());
    int currentTime = ++pwrCheckCount * timer->interval();
    qDebug() << currentTime;
    if (currentTime > SHORT2LONG) {
        emit longPowerKeyPressOccured(true);
    }
}

void EventHandler::hwBtnDown(int code)
{  
    emit powerKeyDownOccured();
    if (powerPressTimer) {
        powerPressTimer->stop();
        powerPressTimer->deleteLater();
    }
    powerPressTimer = new QTimer(this);
    powerPressTimer->setInterval(50);
    connect(powerPressTimer, SIGNAL(timeout()), this, SLOT(pwrKeyHold()));
    pwrCheckCount = 0;
    powerPressTimer->start();

    qDebug() << "down" << code;
}

void EventHandler::hwBtnUp(int code)
{
    emit powerKeyUpOccured();
    if (powerPressTimer == NULL)
        return;

    qDebug() << "up" << code;
    powerPressTimer->stop();
    if (powerPressTimer->interval()*pwrCheckCount < SHORT2LONG) {
        emit shortPowerKeyPressOccured();
    } else {
        emit longPowerKeyPressOccured(false);
    }

    powerPressTimer->deleteLater();
    powerPressTimer = NULL;
}

void EventHandler::alarmClockEvent(qmAlarmCookie)
{
}
