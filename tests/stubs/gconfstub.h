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
#ifndef SYSTEMUIGCONF_H
#define SYSTEMUIGCONF_H

#include <QObject>
#include <QVariant>

class SystemUIGConf : public QObject
{
public:
    enum GConfKeyGroup {
        Battery,
        Display,
        Network
    };

    enum GConfKey {
        BatteryPSMAutoKey,
        BatteryPSMThresholdKey,
        DisplayBrightnessMaxLevelKey,
        DisplayBrightnessLevelStepKey,
        DisplayBrightnessKey,
        DisplayBlankTimeoutsKey,
        DisplayBlankTimeoutKey,
        DisplayDimTimeoutsKey,
        DisplayDimTimeoutKey,
        DisplayBlankInhibitKey,
        NetworkRoamingKey,
        NetworkRoamingUpdatesKey
    };

    SystemUIGConf(QObject *parent = 0);
    ~SystemUIGConf();

    void setValue(SystemUIGConf::GConfKey key, QVariant value);
    QVariant value(SystemUIGConf::GConfKey, QVariant def = NULL);
    int keyCount(SystemUIGConf::GConfKeyGroup keyGroup);

private: //attributes
    QHash<SystemUIGConf::GConfKey, QVariant> GConfValues;

};

#endif // SYSTEMUIGCONF_H
