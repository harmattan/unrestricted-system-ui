/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino=" (0,W2s,i2s,t0,l1,:0" */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef UT_BATTERYBUSINESSLOGIC_H
#define UT_BATTERYBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

#include "batterybusinesslogic.h"
#include "batterystub.h"
#include "devicemodestub.h"
#include "displaystatestub.h"
#include "ledstub.h"

class SignalSink : public QObject
{
    Q_OBJECT
public:
    SignalSink ();
    void reset ();
    void print ();

public slots:
    void batteryCharging (int animationLevel);
    void batteryNotCharging ();
    void batteryBarValueChanged (int barValue);
    
public:
    bool    m_BatteryChargingCame;
    bool    m_BatteryNotChargingCame;
    bool    m_BatteryBarValueCame;

    int     m_AnimationRate;
    int     m_BarValue;
};

class Ut_BatteryBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testBatteryChargingSignal ();
    void testBatteryBarValueChangedSignal ();
    void testSetPSMThreshold();
    void testTogglePSMAuto();
    void testBatteryBarValue();

private:
    SignalSink             m_SignalSink;
    SystemUIGConf         *systemUIGConf;
    BatteryBusinessLogic*  m_subject;
};

#endif
