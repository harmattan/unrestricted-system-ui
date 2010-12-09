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

#ifndef STATUSINDICATORMENUADAPTOR_H
#define STATUSINDICATORMENUADAPTOR_H

#include <QDBusAbstractAdaptor>
#include "statusindicatormenubusinesslogic.h"

/*
 * Adaptor class for interface com.meego.core.MStatusIndicatorMenu
 */
class StatusIndicatorMenuAdaptor : QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.meego.core.MStatusIndicatorMenu")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.meego.core.MStatusIndicatorMenu\">\n"
                "    <method name=\"open\">\n"
                "    </method>\n"
                "  </interface>\n"
                "")

public:
    StatusIndicatorMenuAdaptor(StatusIndicatorMenuBusinessLogic *parent);

    inline StatusIndicatorMenuBusinessLogic *parent() const {
        return static_cast<StatusIndicatorMenuBusinessLogic *>(QObject::parent());
    }

public slots:
    void open();
};

#endif
