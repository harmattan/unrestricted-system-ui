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
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef SYSTEMUIGCONF_H
#define SYSTEMUIGCONF_H

#include <QHash>
#include <QObject>
#include <QVariant>

class DuiGConfItem;

/*!
 * Implements a permanent storage for the settings handled by the sysuid daemon.
 * The configuration items are identified by numerical IDs and stored with the
 * help of the GConf library.
 */
class SystemUIGConf : public QObject
{
    Q_OBJECT
public:

    enum GConfKeyGroup {
        Battery = 0,
        Led
    };

    enum GConfKey {
        BatteryPSMAutoKey       = 0,
        BatteryPSMThresholdKey,
        LedAllEnabled,
	MissedCallLed,
        SMSReceivedLed,
        EmailReceivedLed,
        InstantMessageReceivedLed,
        ChargingLed,
        OtherNotificationsLed,
    };

    SystemUIGConf(QObject* parent = 0);
    virtual ~SystemUIGConf();

    void setValue(SystemUIGConf::GConfKey key, QVariant value);
    QVariant value(SystemUIGConf::GConfKey, QVariant def = QVariant());
    int keyCount(SystemUIGConf::GConfKeyGroup keyGroup);

signals:
    void valueChanged(SystemUIGConf::GConfKey key, QVariant value);

private slots:
    void keyValueChanged();

private: //methods
    QString mapGConfKeyGroup(SystemUIGConf::GConfKeyGroup keyGroup);
    QString mapGConfKey(SystemUIGConf::GConfKey key);

private: //attributes
    QHash<SystemUIGConf::GConfKey, DuiGConfItem *> duiGConfItems;

};


#endif
