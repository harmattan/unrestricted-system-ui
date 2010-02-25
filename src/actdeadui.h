/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef ACTDEADUI_H
#define ACTDEADUI_H

#include "actdeadbusinesslogic.h"
class DuiApplicationPage;
class DuiImageWidget;
class DuiLabel;
class QTimer;

class ActDeadUI : public QObject
{
    Q_OBJECT

    public:
        ActDeadUI (QObject *parent = 0);
        ~ActDeadUI ();
    
    public slots:
        void chargingComplete ();
        void powersave (bool active);
        void animate ();

    private:
        void showCharging ();
        void showNotCharging ();
        void createContents ();

    private:
        DuiApplicationPage      *m_window;
        ActDeadBusinessLogic    *m_logic;
        DuiImageWidget          *m_anim;
        QStringList             *m_anim_imgs;
        QTimer                  *m_timer;
        DuiLabel                *m_status;
};

#endif

