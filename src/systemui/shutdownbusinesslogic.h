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
#ifndef SHUTDOWNBUSINESSLOGIC_H
#define SHUTDOWNBUSINESSLOGIC_H

#ifdef HAVE_QMSYSTEM
#include <qmsystemstate.h>
#endif

#include <QDBusAbstractAdaptor>
#include <QObject>

class ShutdownUI;

/*!
 * \brief Business logic for the shutdown subsystem. 
 *
 * This class connects the systemStateChanged() signal of the QmSystem library
 * and notify the user about the state changes. The class does nothing else.
 * The messages are sent, when:
 * 1) Shutdown is happening.
 * 2) Thermal emergency shutdown is happening.
 * 3) Battery low shutdown is happening.
 * 4) The shutdown is aborted because the USB is connected.
 *
 */
class ShutdownBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ShutdownBusinessLogic(QObject *parent = 0);
    virtual ~ShutdownBusinessLogic ();

    void showUI(QString text1 = QString (""), QString text2 = QString (""), int timeout = 2000);

public slots:
#ifdef HAVE_QMSYSTEM
    /*!
     * This function is called when the QmSystem reports a state change.
     */
    void systemStateChanged (MeeGo::QmSystemState::StateIndication what);
#endif

private:
    void thermalShutdown();
    void batteryShutdown();
    void shutdownDeniedUSB();
    void createAndPublishNotification(const QString &type, const QString &summary, const QString &body);

private:
    //! Window that shows the shutdown information
    ShutdownUI *shutdownUi;

#ifdef HAVE_QMSYSTEM
    MeeGo::QmSystemState   *m_State;
#endif

#ifdef UNIT_TEST
    friend class Ft_ShutdownBusinessLogic;
    friend class Ut_ShutdownBusinessLogic;
#endif
};

class ShutdownBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.shutdownui")

public:
    ShutdownBusinessLogicAdaptor (
            QObject                  *parent,
            ShutdownBusinessLogic    *logic);

public slots:
    Q_NOREPLY void showScreen (QString text1, QString text2, int timeout);

private:
    ShutdownBusinessLogic *shutdownBusinessLogic;
#ifdef UNIT_TEST
    friend class Ft_ShutdownBusinessLogic;
    friend class Ut_ShutdownBusinessLogic;
#endif
};

#endif
