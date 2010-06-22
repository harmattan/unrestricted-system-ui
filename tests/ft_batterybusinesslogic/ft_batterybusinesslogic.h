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
#ifndef FT_BATTERYBUSINESSLOGIC_H
#define FT_BATTERYBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

#include "batterybusinesslogic.h"
#include "batterystub.h"
#include "devicemodestub.h"
#include "displaystatestub.h"
#include "ledstub.h"

#include "../ft_lockscreenui/xchecker.h"

class QString;

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
    void notificationSent (
            QString      text,
            QString      icon);

public:
    bool       m_NotificationCame;
    QString    m_NotificationText;
    QString    m_NotificationIcon;
    bool       m_BatteryChargingCame;
    bool       m_BatteryNotChargingCame;
    bool       m_BatteryBarValueCame;

    int        m_AnimationRate;
    int        m_BarValue;
};

class Ft_BatteryBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

#ifdef HAVE_QMSYSTEM
    void testEnterPSM ();
    void testLeavePSM ();
    void testChargingComplete ();
    void testCharging ();
    void testChargingFailed ();
    void testLowBattery ();
    void testRemoveCharger ();
#endif
    
private:
    SignalSink             m_SignalSink;
    XChecker               m_XChecker;
    BatteryBusinessLogic  *m_Subject;
};

#endif

