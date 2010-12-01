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

#include "ngfadapter.h"

#ifdef HAVE_LIBNGF
#include <dbus/dbus-glib-lowlevel.h>

NGFAdapter::NGFAdapter()
{
    connection = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    dbus_connection_setup_with_g_main(connection, NULL);

    client = ngf_client_create(NGF_TRANSPORT_DBUS, connection);
}

NGFAdapter::~NGFAdapter()
{
    ngf_client_destroy(client);
    dbus_connection_unref(connection);
}

uint NGFAdapter::play(const QString &id)
{
    uint eventId = 0;

    if (isValid() && !id.isEmpty()) {
        NgfProplist *p = NULL;
        eventId = ngf_client_play_event (client, id.toUtf8().constData(), p);
    }

    return eventId;
}

void NGFAdapter::stop(uint eventId)
{
    if (eventId > 0) {
        ngf_client_stop_event (client, eventId);
    }
}


bool NGFAdapter::isValid()
{
    return client != NULL;
}
#else
NGFAdapter::NGFAdapter()
{
}

NGFAdapter::~NGFAdapter()
{
}

uint NGFAdapter::play(const QString &id)
{
    Q_UNUSED (id);
    return 0;
}

void NGFAdapter::stop(uint eventId)
{
    Q_UNUSED (eventId);
}

bool NGFAdapter::isValid()
{
    return true;
}
#endif
