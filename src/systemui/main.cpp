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
#include <MApplicationService>
#include <signal.h>

sighandler_t originalSigIntHandler = NULL;

void sigIntHandler(int)
{
    if (qApp != NULL) {
        qApp->quit();
    }
}

class SystemUIservice : public MApplicationService
{
public:
    SystemUIservice(QObject *parent = 0) : MApplicationService("com.nokia.sysuid", parent)
    {
    }

    void launch()
    {
        // No operation, we must not let meegotouch to raise/activate some hidden sysuid window
    }
};

int main(int argc, char** argv)
{
    originalSigIntHandler = signal(SIGINT, sigIntHandler);

    MApplication app(argc, argv, new SystemUIservice);
    app.setQuitOnLastWindowClosed(false);

    Sysuid daemon(&app);

    return app.exec();
}
