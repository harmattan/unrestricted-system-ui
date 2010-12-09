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

#include "ut_statusindicatormenuadaptor.h"

#include <MApplication>
#include <MApplicationPage>
#include <MEscapeButtonPanel>
#include "statusindicatormenuadaptor.h"
#include "statusindicatormenubusinesslogic_stub.h"

void Ut_StatusIndicatorMenuAdaptor::init()
{
    menuBusinessLogic = new StatusIndicatorMenuBusinessLogic();
    menuAdaptor = new StatusIndicatorMenuAdaptor(menuBusinessLogic);
    gStatusIndicatorMenuBusinessLogicStub->stubReset();
}

void Ut_StatusIndicatorMenuAdaptor::cleanup()
{
    delete menuAdaptor;
    delete menuBusinessLogic;
}

void Ut_StatusIndicatorMenuAdaptor::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static char *args[] = {(char *)"./ut_statusindicator"};
    static int argc = sizeof(args) / sizeof(char*);

    app = new MApplication(argc, args);

}

void Ut_StatusIndicatorMenuAdaptor::cleanupTestCase()
{
    // Destroy the MApplication
    delete app;
}

void Ut_StatusIndicatorMenuAdaptor::testOpenSignal()
{
    // When the adaptor open() method is called, the menu window
    // should be made visible via the makeVisible() method call.
    menuAdaptor->open();
    QCOMPARE(gStatusIndicatorMenuBusinessLogicStub->stubCallCount("showStatusIndicatorMenu"), 1);
}


QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuAdaptor)
