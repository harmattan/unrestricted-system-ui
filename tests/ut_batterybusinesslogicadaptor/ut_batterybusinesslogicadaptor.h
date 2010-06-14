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
#ifndef _UT_BATTERYBUSINESSLOGICADAPTOR_H
#define  _UT_BATTERYBUSINESSLOGICADAPTOR_H

#include <QObject>

class BatteryBusinessLogic;
class BatteryBusinessLogicAdaptor;

class Ut_BatteryBusinessLogicAdaptor : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase ();
    void cleanupTestCase ();

    void setPSMValue ();
    void setPSMAutoValue ();
    void setPSMThresholdValue ();
    void remainingTimeValues ();
    void PSMThresholdValues ();
    void batteryChargingState ();
    void batteryBarValue ();

private:
    BatteryBusinessLogic *m_logic;
    BatteryBusinessLogicAdaptor *m_subject;
};

#endif
