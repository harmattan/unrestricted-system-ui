/*
 * profileplugin.h
 *
 * This file is part of duistatusindicatormenu
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

#include <DuiStatusIndicatorMenuPluginInterface>
#include <QObject>

// This class doesn't do anything else but creates a widget
class ProfilePlugin : public QObject, public DuiStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiStatusIndicatorMenuPluginInterface)

public:
    // Methods derived from DuiStatusIndicatorMenuPlugin
    virtual DuiWidget *constructWidget (
        DuiStatusIndicatorMenuInterface &statusIndicatorMenu);
};

#endif // PROFILEPLUGIN_H
