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
#include "gconfstub.h"

SystemUIGConf::SystemUIGConf(QObject* parent) :
        QObject(parent)
{
}

SystemUIGConf::~SystemUIGConf()
{
}

int SystemUIGConf::keyCount(SystemUIGConf::GConfKeyGroup keyGroup)
{
    Q_UNUSED(keyGroup);
    return 0;
}

void SystemUIGConf::setValue(SystemUIGConf::GConfKey key, QVariant value)
{
    GConfValues.insert(key, value);
}

QVariant SystemUIGConf::value(SystemUIGConf::GConfKey key, QVariant def)
{
    if (GConfValues.contains(key))
        return GConfValues.value(key);
    else
        return def;
}
