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
#include <QObject>
#include <signal.h>

MApplication *exitPtr;

void sysuid_exit(int)
{
    if (exitPtr != NULL) {
        exitPtr->quit();
        exitPtr = NULL;
    }
}

class SystemUIservice : public MApplicationService
{
public:
    SystemUIservice(QObject *parent = 0) :
        MApplicationService("com.nokia.sysuid", parent)
    {
    }

    void launch()
    {
        /*
         * No operation, we must not let meegotouch to
         * raise/activate some hidden sysuid window.
         */
    }
};

int main(int argc, char** argv)
{
    signal(SIGINT, sysuid_exit);

    MApplication app(argc, argv, new SystemUIservice);
    exitPtr = &app;

    app.setQuitOnLastWindowClosed(false);

    Sysuid daemon(&app);

    return app.exec();
}
