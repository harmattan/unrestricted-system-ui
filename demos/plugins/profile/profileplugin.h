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
/*
 * profileplugin.h
 *
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#ifndef PROFILEPLUGIN_H
#define PROFILEPLUGIN_H

#include <MStatusIndicatorMenuPluginInterface>
#include <QObject>

// This class doesn't do anything else but creates a widget
class ProfilePlugin : public QObject, public MStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(MStatusIndicatorMenuPluginInterface)

public:
    // Methods derived from MStatusIndicatorMenuPlugin
    virtual MWidget *constructWidget(MStatusIndicatorMenuInterface &statusIndicatorMenu);
};

#endif // PROFILEPLUGIN_H
