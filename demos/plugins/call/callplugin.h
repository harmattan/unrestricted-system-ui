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

#ifndef CALLPLUGIN_H
#define CALLPLUGIN_H

#include <MStatusIndicatorMenuExtensionInterface>
#include <QObject>

class Call;
class MStatusIndicatorMenuInterface;

class CallPlugin : public QObject, public MStatusIndicatorMenuExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(MStatusIndicatorMenuExtensionInterface MApplicationExtensionInterface)

public:
    CallPlugin();

    // Getter for the status indicator menu interface
    MStatusIndicatorMenuInterface *statusIndicatorMenuInterface() const;

    // Methods derived from MStatusIndicatorMenuPlugin
    virtual void setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &menuInterface);

    // Methods derived from MApplicationExtensionInterface
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();

private:
    MStatusIndicatorMenuInterface *statusIndicatorMenu;
    Call *call;
};

#endif // CALLPLUGIN_H
