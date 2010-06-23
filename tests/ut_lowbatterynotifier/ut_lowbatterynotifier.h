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
#ifndef UT_LOWBATTERYNOTIFIER_H
#define UT_LOWBATTERYNOTIFIER_H

#include <QtTest/QtTest>
#include <QObject>

#include "batterybusinesslogic.h"

class QTime;

class LowBatteryHelper : public QObject
{
    Q_OBJECT

public:
    LowBatteryHelper() : QObject() {
        ;
    }
    void start();
    QList<int> notificationTimes();

public slots:
    void notificationShown();

private: // attributes
    QList<int> times;
    QTime time;

};

class Ut_LowBatteryNotifier : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

#ifdef HAVE_QMSYSTEM
    void testShowNotificationInActiveUse();
    void testShowNotificationInDiverseUse();
    void testShowNotificationInInactiveUse();
#endif

private:
#ifdef HAVE_QMSYSTEM
    void turnDisplay (bool On);
#endif

    LowBatteryNotifier  *m_subject;
    LowBatteryHelper    *m_helper;
    QTime time;
    int shown;
};

#endif
