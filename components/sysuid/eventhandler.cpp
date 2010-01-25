/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <QDebug>

// last include, please
#include "eventhandler.h"

#define DEBUG
#include "../debug.h"

const int SHORT2LONG = 2000/*ms*/;

EventHandler::EventHandler (
		QObject *parent) :
	QObject(parent)
{
    m_PowerPressTimer = NULL;

    m_Keys = new QmKeys ();
#if 0
    alarm = new QmAlarm();
#endif

    //Monitor hardware key presses
    connect(m_Keys, SIGNAL(keyPressed(QmKeys::Keys, QmKeys::HowPressed)),
            this, SLOT(keyPressed(QmKeys::Keys, QmKeys::HowPressed)));

#if 0
    //Monitor alarms
    connect(alarm, SIGNAL(ring(qmAlarmCookie)), 
            this, SLOT(alarmClockEvent(qmAlarmCookie)));
#endif

    connect (m_Keys, SIGNAL(keyDown(int)), this, SLOT(hwBtnDown(int)));
    connect (m_Keys, SIGNAL(keyUp(int)), this, SLOT(hwBtnUp(int)));
}

EventHandler::~EventHandler()
{
    delete m_Keys;
    m_Keys = NULL;
#if 0
    delete alarm;
    alarm = NULL;
#endif
}


void 
EventHandler::keyPressed (
        QmKeys::Keys         key, 
        QmKeys::HowPressed   how)
{
    switch (key) {
        case QmKeys::PowerKey:
            SYS_DEBUG ("QmKeys::PowerKey");
            switch (how) {
                case QmKeys::ShortPress:
                    emit shortPowerKeyPressOccured();
                    break;
                case QmKeys::LongPress:
                    break;
            }
        break;

        case QmKeys::HomeKey:
            SYS_DEBUG ("QmKeys::HomeKey");
            break;

        case QmKeys::CameraShutterKey:
            SYS_DEBUG ("QmKeys::CameraShutterKey");
            break;

        default:
            SYS_DEBUG ("Unknown key");
            break;
    }
}

void 
EventHandler::pwrKeyHold ()
{
    QTimer *timer = static_cast<QTimer*>(this->sender());
    int currentTime = ++m_PwrCheckCount * timer->interval ();
    qDebug() << currentTime;
    if (currentTime > SHORT2LONG) {
        emit longPowerKeyPressOccured(true);
    }
}

void
EventHandler::hwBtnDown (
        int code)
{
    emit powerKeyDownOccured ();
    if (m_PowerPressTimer) {
        m_PowerPressTimer->stop ();
        m_PowerPressTimer->deleteLater ();
    }

    m_PowerPressTimer = new QTimer (this);
    m_PowerPressTimer->setInterval (50);
    connect (m_PowerPressTimer, SIGNAL(timeout()), 
            this, SLOT(pwrKeyHold()));
    m_PwrCheckCount = 0;
    m_PowerPressTimer->start ();

    qDebug() << "down" << code;
}

void 
EventHandler::hwBtnUp (
        int code)
{
    emit powerKeyUpOccured ();
    if (m_PowerPressTimer == NULL)
        return;

    qDebug() << "up" << code;
    m_PowerPressTimer->stop ();
    if (m_PowerPressTimer->interval() * m_PwrCheckCount < SHORT2LONG) {
        emit shortPowerKeyPressOccured ();
    } else {
        emit longPowerKeyPressOccured (false);
    }

    m_PowerPressTimer->deleteLater ();
    m_PowerPressTimer = NULL;
}

#if 0
void 
EventHandler::alarmClockEvent (
        qmAlarmCookie)
{
}
#endif
