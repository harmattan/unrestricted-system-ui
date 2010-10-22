
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

void Ut_StatusIndicatorMenu::testWhenPressedOutsideMenuContentsThenWindowShouldHide()
{
    MButton *widget = new MButton(statusIndicatorMenu);

    TestGraphicsScene scene;
    MSceneManager sceneManager(&scene);

    statusIndicatorMenu->appear(&scene);

    statusIndicatorMenu->setGeometry(QRectF(0, 0, 1000, 1000));
    widget->setGeometry(QRectF(20, 20, 100, 100));

    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    event.setButton(Qt::LeftButton);
    QGraphicsSceneMouseEvent revent(QEvent::GraphicsSceneMouseRelease);
    revent.setButton(Qt::LeftButton);

    QSignalSpy hideSpy(statusIndicatorMenu, SIGNAL(hideRequested()));

    // Click inside the content widget
    event.setScenePos(QPointF(widget->geometry().x()+5, widget->geometry().y()+5));
    revent.setScenePos(QPointF(widget->geometry().x()+5, widget->geometry().y()+5));
    scene.event(&event);
    scene.event(&revent);

    // The window shouldn't close yet
    QCOMPARE(hideSpy.count(), 0);

    // Press inside the content widget but release outside
    event.setScenePos(QPointF(widget->geometry().x()+5, widget->geometry().y()+5));
    revent.setScenePos(QPointF(widget->geometry().x()-5, widget->geometry().y()-5));
    scene.event(&event);
    scene.event(&revent);

    // The window still shouldn't close
    QCOMPARE(hideSpy.count(), 0);

    // Click outside the content widget
    event.setScenePos(QPointF(widget->geometry().x()-5, widget->geometry().y()-5));
    revent.setScenePos(QPointF(widget->geometry().x()-5, widget->geometry().y()-5));
    scene.event(&event);
    scene.event(&revent);

    // Now the window should close
    QCOMPARE(hideSpy.count(), 1);

    // Click outside the content widget and release inside
    event.setScenePos(QPointF(widget->geometry().x()-5, widget->geometry().y()-5));
    revent.setScenePos(QPointF(widget->geometry().x()+5, widget->geometry().y()+5));
    scene.event(&event);
    scene.event(&revent);

    // The window should close again
    QCOMPARE(hideSpy.count(), 2);

    // Re-gain ownership of statusIndicatorMenu
    scene.removeItem(statusIndicatorMenu);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenu)
