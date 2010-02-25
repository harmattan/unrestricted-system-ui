/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef ACTDEADBUSINESSLOGIC_H
#define ACTDEADBUSINESSLOGIC_H

#include <QObject>
#include <qmsystem/qmbattery.h>
#include <qmsystem/qmdisplaystate.h>

using namespace Maemo;

class ActDeadBusinessLogic : public QObject
{
    Q_OBJECT

    public:
        ActDeadBusinessLogic (QObject *parent = 0);
        QmBattery::State getBatteryState ();
        QmBattery::Level getBatteryLevel ();
        void turnOnDisplay ();

    signals:
        void ChargingComplete ();
        void BatteryEvent (QmBattery::State state);
        void PowerSave (bool active);

    private slots:
        void levelChange (QmBattery::Level level);
        void stateChange (QmBattery::State state);
        void displayChange (QmDisplayState::DisplayState state);

    private:
        QmBattery           *m_battery;
        QmDisplayState      *m_display;
        QmBattery::State     m_state;
        QmBattery::Level     m_level;
};

#endif

