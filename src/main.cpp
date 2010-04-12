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
#include "sysuid.h"

#include <MApplication>

#include <QObject>

#undef DEBUG
#include "debug.h"

#include "signal.h"

static MApplication *exitPtr;

void sysuid_exit (int sig)
{
    Q_UNUSED (sig);
    if (exitPtr) {
        exitPtr->quit ();
        exitPtr = NULL;
    }
}

int main (int argc, char** argv)
{
    MApplication app (argc, argv);
    exitPtr = &app;

    qInstallMsgHandler (0);

    app.setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("- System-UI start");

    signal (SIGINT, sysuid_exit);

    Sysuid daemon;

    SYS_DEBUG ("- System-UI daemon initialized");

    // re-install the translations on locale settings changed signal
    QObject::connect (&app, SIGNAL (localeSettingsChanged ()),
                      &daemon, SLOT (retranslate ()));

    int ret = app.exec ();

    return ret;
}

