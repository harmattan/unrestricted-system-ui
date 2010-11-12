
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
#include "ut_statusindicatormenu.h"
#include "statusindicatormenu.h"
#include <MApplication>
#include <MApplicationIfProxy>
#include <MWidgetController>
#include <MScene>
#include <MSceneManager>
#include <QtTest/QtTest>
#include <MButton>

class TestStatusIndicatorMenuExtensionInterface : public MStatusIndicatorMenuExtensionInterface
{
public:
    MStatusIndicatorMenuInterface *menuInterface;

    TestStatusIndicatorMenuExtensionInterface() : menuInterface(NULL) {}

    bool initialize(const QString&)
    {
        return true;
    }

    void setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &mI)
    {
        menuInterface = &mI;
    }
};

class TestGraphicsScene : public MScene
{
public:
    TestGraphicsScene() : MScene() {}

    bool event(QEvent * event)
    {
        return MScene::event(event);
    }
};

// MApplicationIfProxy stubs (used by StatusIndicatorMenu)
bool mApplicationIfProxyLaunchCalled = false;
QDBusPendingReply<> MApplicationIfProxy::launch()
{
    mApplicationIfProxyLaunchCalled = true;

    return QDBusPendingReply<>();
}


void Ut_StatusIndicatorMenu::init()
{
    statusIndicatorMenu = new StatusIndicatorMenu();
    connect(this, SIGNAL(settingsButtonClicked()), statusIndicatorMenu, SLOT(launchControlPanelAndHide()));
    connect(this, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), statusIndicatorMenu, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*)));
}

void Ut_StatusIndicatorMenu::cleanup()
{
    delete statusIndicatorMenu;
}

void Ut_StatusIndicatorMenu::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatormenu";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicatorMenu::cleanupTestCase()
{
    delete app;
}


void Ut_StatusIndicatorMenu::testSettingsButtonClicked()
{
    emit settingsButtonClicked();

    QVERIFY(mApplicationIfProxyLaunchCalled);
}

void Ut_StatusIndicatorMenu::testExtensionInstantiated()
{
    TestStatusIndicatorMenuExtensionInterface extension;

    emit extensionInstantiated(&extension);

    QCOMPARE(extension.menuInterface, statusIndicatorMenu);
}

void Ut_StatusIndicatorMenu::testSignals()
{
    QSignalSpy showSpy(statusIndicatorMenu, SIGNAL(showRequested()));
    statusIndicatorMenu->showStatusIndicatorMenu();
    QCOMPARE(showSpy.count(), 1);

    QSignalSpy hideSpy(statusIndicatorMenu, SIGNAL(hideRequested()));
    statusIndicatorMenu->hideStatusIndicatorMenu();
    QCOMPARE(hideSpy.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenu)
