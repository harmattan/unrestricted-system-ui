/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef ACTDEADUI_H
#define ACTDEADUI_H

#include "actdeadbusinesslogic.h"
class DuiApplicationPage;

class ActDeadUI : public QObject
{
    Q_OBJECT

    public:
        ActDeadUI (QObject *parent = 0);
        ~ActDeadUI ();
    
    public slots:
        void chargingComplete ();
        void batteryEvent (QmBattery::State);

    private:
        void showCharging ();
        void showNotCharging ();
        void createContents ();

    private:
        DuiApplicationPage      *m_window;
        ActDeadBusinessLogic    *m_logic;
};

#endif

