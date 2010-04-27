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

NGFAdapter::NGFAdapter()
{
    connection = dbus_bus_get(DBUS_BUS_SESSION, NULL);
    dbus_connection_setup_with_g_main(connection, NULL);

    client = ngf_client_create(NGF_TRANSPORT_DBUS, connection);
}

NGFAdapter::~NGFAdapter()
{
    ngf_client_destroy(client);
    dbus_connection_unref(connection);
}

void NGFAdapter::play(const QString &id)
{
    if (isValid() && !id.isEmpty()) {
        NgfProplist *p = NULL;
        ngf_client_play_event (client, id.toUtf8().constData(), p);
    }
}

bool NGFAdapter::isValid()
{
    return client != NULL;
}
