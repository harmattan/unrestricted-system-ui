/***************************************************************************
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
#include <stdint.h>
#include "ut_ngfadapter.h"

static uint32_t g_ngf_play_id = 0;
static bool g_ngf_play_called = false;
static bool g_ngf_stop_called = false;
static bool g_ngf_create_client = false;
static QString* g_ngf_event_id;
static uint32_t g_ngf_stop_id;

#ifdef HAVE_LIBNGF
uint32_t ngf_client_play_event (NgfClient *client,
                                const char *event,
                                NgfProplist *proplist)
{
    Q_UNUSED(client)
    Q_UNUSED(proplist)

    g_ngf_play_called = true;
    g_ngf_event_id = new QString(event);
    return ++g_ngf_play_id;
}

void ngf_client_stop_event (NgfClient *client,
                            uint32_t id)
{
    Q_UNUSED(client)
    g_ngf_stop_called = true;
    g_ngf_stop_id = id;
}


NgfClient* ngf_client_create (NgfTransport transport, ...)
{
    Q_UNUSED(transport)

    NgfClient* client = NULL;
    if (g_ngf_create_client) {
        client = (NgfClient*) malloc(sizeof(NgfClient*));
        return client;
    }
    return client;
}

void ngf_client_destroy (NgfClient *client)
{
    if (client) {
        free(client);
    }
}
#endif

void Ut_NGFAdapter::init() {
    g_ngf_play_id = 0;
    g_ngf_play_called = false;
    g_ngf_stop_called = false;
    g_ngf_create_client = true;
    adapter = new NGFAdapter;
    QVERIFY(adapter->isValid());
}

void Ut_NGFAdapter::cleanup()
{
    delete adapter;

    if (g_ngf_event_id) {
        delete g_ngf_event_id;
        g_ngf_event_id = NULL;
    }
}

void Ut_NGFAdapter::testPlayAndStop()
{
    QVERIFY(!g_ngf_play_called);
    QVERIFY(!g_ngf_event_id);

    const QString event_id("my-id");
    uint32_t id = adapter->play(event_id);

#ifdef HAVE_LIBNGF
    // Check that the event was played using the NGF framework and that
    // the event identifier was the same ar that passed to adapter->play()
    QVERIFY(g_ngf_play_called);
    QCOMPARE(event_id.toUtf8(), g_ngf_event_id->toUtf8());
#endif

    adapter->stop(id);

#ifdef HAVE_LIBNGF
    // Check that the event was stopped using the NGF framework and that
    // the id was that same one that ngf_client_play_event() returned
    QVERIFY(g_ngf_stop_called);
    QCOMPARE(id, g_ngf_stop_id);
#endif
}

void Ut_NGFAdapter::testNgfClientCreationFails()
{
    QVERIFY(!g_ngf_play_called);

    // Delete adapter created by init as we want to contruct it differently
    delete adapter;
    g_ngf_create_client = false;
    adapter = new NGFAdapter;

#ifdef HAVE_LIBNGF
    // Check that the adapter is not valid as the client creation
    // should have failed
    QVERIFY(!adapter->isValid());

    const QString event_id("id");
    uint32_t id = adapter->play(event_id);

    // The play should never happen as we do not have a valid client
    QVERIFY(!g_ngf_play_called);

    adapter->stop(id);

    // The play should never happen as we do not have a valid client
    QVERIFY(!g_ngf_stop_called);
#endif
}

QTEST_APPLESS_MAIN(Ut_NGFAdapter)
