/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "actdeadbusinesslogic.h"

#define DEBUG
#define WARNING
#include "debug.h"

// Yeah i know, this class is not so usefull at this state
// but in the future i think we should handle the touch-screen
// locking here... <dkedves>

ActDeadBusinessLogic::ActDeadBusinessLogic (QObject *parent) :
    QObject (parent),
    m_battery (0),
    m_display (0)
{
    SYS_DEBUG ("");
    m_battery = new QmBattery (this);
    m_state = m_battery->getState ();
    m_level = m_battery->getLevel ();
    m_display = new QmDisplayState (this);

    connect (m_battery, SIGNAL (batteryLevelChanged (QmBattery::Level)),
             this, SLOT (levelChange (QmBattery::Level)));
    connect (m_battery, SIGNAL (batteryStatusChanged (QmBattery::State)),
             this, SLOT (stateChange (QmBattery::State)));

    // Turn on screen-blanking when charging
    // in Active Dead mode
    m_display->setBlankingWhenCharging (true);
    // Set some default timeouts
    m_display->setDisplayDimTimeout (60);
    m_display->setDisplayBlankTimeout (61);

    // Dim the display (maybe the charging
    // animation and the window isn't ready)
    m_display->set (QmDisplayState::Dimmed);
}

ActDeadBusinessLogic::~ActDeadBusinessLogic ()
{
    delete m_battery;
    m_battery = 0;
    delete m_display;
    m_display = 0;
}

QmBattery::State
ActDeadBusinessLogic::getBatteryState ()
{
    return m_state;
}

QmBattery::Level
ActDeadBusinessLogic::getBatteryLevel ()
{
    return m_level;
}

void
ActDeadBusinessLogic::levelChange (QmBattery::Level level)
{
    m_level = level;

#ifdef DEBUG
    switch (m_level)
    {
        case QmBattery::LevelFull:
            SYS_DEBUG ("Battery full");       
            break;
        case QmBattery::LevelLow:
            SYS_DEBUG ("Battery low");
            break;
        case QmBattery::LevelCritical:
            SYS_DEBUG ("Battery level critically low");
            break;
    }
#endif

    emit levelChange (m_level);
}

void
ActDeadBusinessLogic::stateChange (QmBattery::State state)
{
    m_state = state;

    switch (m_state)
    {
        case QmBattery::StateChargingFailed:
            SYS_DEBUG ("Charging failed");
            // TODO: We maybe should emit some signal here,
            // so UI can show something when this happening
            break;
        case QmBattery::StateNotCharging:
            SYS_DEBUG ("Not charging");
            if (m_level == QmBattery::LevelFull)
                emit ChargingComplete ();
            break;
        case QmBattery::StateCharging:
            SYS_DEBUG ("Charging");
            break;
    }

    emit stateChange (m_state);
}

void
ActDeadBusinessLogic::turnOnDisplay ()
{
    SYS_DEBUG ("");
    // Turn on the display
    m_display->set (QmDisplayState::On);
    // Pause blanking for a while...
    m_display->setBlankingPause ();
}

