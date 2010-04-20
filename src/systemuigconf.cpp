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
#include "systemuigconf.h"

#include <MGConfItem>
#include <QDebug>

#undef DEBUG
#include "debug.h"

SystemUIGConf::SystemUIGConf (
		QObject* parent) :
	QObject(parent)
{
    // init the gconf keys
    mGConfItems.insert (SystemUIGConf::BatteryPSMAutoKey, 
            new MGConfItem (mapGConfKey(SystemUIGConf::BatteryPSMAutoKey)));

    mGConfItems.insert (SystemUIGConf::BatteryPSMThresholdKey, 
            new MGConfItem (
                mapGConfKey(SystemUIGConf::BatteryPSMThresholdKey)));
    
    mGConfItems.insert (SystemUIGConf::LedAllEnabled, 
            new MGConfItem (
                mapGConfKey(SystemUIGConf::LedAllEnabled)));
    
    mGConfItems.insert (SystemUIGConf::MissedCallLed, 
            new MGConfItem (mapGConfKey(SystemUIGConf::MissedCallLed)));
    mGConfItems.insert (SystemUIGConf::SMSReceivedLed, 
            new MGConfItem (mapGConfKey(SystemUIGConf::SMSReceivedLed)));
    mGConfItems.insert (SystemUIGConf::EmailReceivedLed, 
            new MGConfItem (mapGConfKey(SystemUIGConf::EmailReceivedLed)));
    mGConfItems.insert (SystemUIGConf::InstantMessageReceivedLed, 
            new MGConfItem (mapGConfKey(SystemUIGConf::InstantMessageReceivedLed)));
    mGConfItems.insert (SystemUIGConf::ChargingLed, 
            new MGConfItem (mapGConfKey(SystemUIGConf::ChargingLed)));
    mGConfItems.insert (SystemUIGConf::OtherNotificationsLed, 
            new MGConfItem (mapGConfKey(SystemUIGConf::OtherNotificationsLed)));
    
    QHash<SystemUIGConf::GConfKey, MGConfItem *>::iterator i;
    for (i = mGConfItems.begin(); i != mGConfItems.end(); ++i)
        connect (i.value (), SIGNAL (valueChanged()), 
                this, SLOT (keyValueChanged()));

}

SystemUIGConf::~SystemUIGConf()
{
    QHash<SystemUIGConf::GConfKey, MGConfItem *>::iterator i;
    for (i = mGConfItems.begin(); i != mGConfItems.end(); ++i) {
        delete i.value();
        i.value() = NULL;
    }
}


int
SystemUIGConf::keyCount (
        SystemUIGConf::GConfKeyGroup keyGroup)
{
    MGConfItem mGConfItem(mapGConfKeyGroup(keyGroup));
    QList<QString> list = mGConfItem.listEntries();

    return list.size();
}

/*!
 * Sets a configuration item value by its numerical id.
 */
void
SystemUIGConf::setValue (
        SystemUIGConf::GConfKey key, 
        QVariant                value)
{
    SYS_DEBUG ("*** key      = %d", (int) key);
    SYS_DEBUG ("*** value    = %s", SYS_BOOL(value.toBool()));

    mGConfItems.value(key)->set (value);
}

/*!
 * Returns a configuration item value by its numerical ID and default value.
 */
QVariant 
SystemUIGConf::value (
        SystemUIGConf::GConfKey key, 
        QVariant                def)
{
    return mGConfItems.value(key)->value (def);
}

/*!
 * Callback function that is called when the value of some GConf item has been
 * changed. 
 */
void 
SystemUIGConf::keyValueChanged ()
{
    MGConfItem *item = static_cast<MGConfItem *>(this->sender());
    emit valueChanged(mGConfItems.key(item), item->value());
}

/*!
 * Maps a numerical configuration group id to a gconf key.
 */
QString
SystemUIGConf::mapGConfKeyGroup (
        SystemUIGConf::GConfKeyGroup keyGroup)
{
    QString keyGroupStr;

    switch (keyGroup) {
        case SystemUIGConf::Battery:
            keyGroupStr = "/systemui/settings/battery";
            break;

        case SystemUIGConf::Led:
            keyGroupStr = "/systemui/settings/leds";
            break;

        default:
            break;
    }

    return keyGroupStr;
}

/*!
 * Maps a numerical configuration id to a GConf key used to store the value.
 */
QString
SystemUIGConf::mapGConfKey (
        SystemUIGConf::GConfKey key)
{
    QString keyStr;

    switch (key) {
        case SystemUIGConf::BatteryPSMAutoKey:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Battery) +
                    "/batteryPSMAuto";
            break;

        case SystemUIGConf::BatteryPSMThresholdKey:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Battery) +
                    "/batteryPSMThreshold";
            break;

        case SystemUIGConf::LedAllEnabled:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/allLedsEnabled";
            break;
        
        case SystemUIGConf::MissedCallLed:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/MissedCallLedEnabled";
            break;
        
	case SystemUIGConf::SMSReceivedLed:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/SMSReceivedLedEnabled";
            break;
        
	case SystemUIGConf::EmailReceivedLed:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/EmailReceivedLedEnabled";
            break;
        
	case SystemUIGConf::InstantMessageReceivedLed:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/InstantMessageReceivedLedEnabled";
            break;
        
	case SystemUIGConf::ChargingLed:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/ChargingLedEnabled";
            break;
	
	case SystemUIGConf::OtherNotificationsLed:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/OtherNotificationsLed";
            break;

        default:
            break;
    }

    SYS_DEBUG ("Returning '%s'", SYS_STR(keyStr));
    return keyStr;
}

