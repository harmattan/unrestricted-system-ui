
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

#include <QtTest/QtTest>
#include "ut_statusindicatormenuverticalview.h"
#include "statusindicatormenuverticalview.h"
#include "statusindicatormenu.h"
#include <MApplication>
#include <MApplicationExtensionArea>
#include "statusindicatormenustyle.h"
#include <MSceneManager>
#include <MButton>
#include <QGraphicsLayout>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include "x11wrapper_stub.h"

#ifdef HAVE_QMSYSTEM
#include "qmlocks_stub.h"
#endif

// MApplicationExtensionArea stubs
QString mApplicationExtensionAreaInterface;
MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, QGraphicsItem *)
{
    mApplicationExtensionAreaInterface = interface;
}

MApplicationExtensionArea::~MApplicationExtensionArea() { }

QRegExp mApplicationExtensionAreaInProcessFilter;
QRegExp mApplicationExtensionAreaVerticalFilter;
void MApplicationExtensionArea::setInProcessFilter(const QRegExp &inProcessFilter)
{
    if (inProcessFilter == QRegExp("/statusindicatormenu-(call|transfer).desktop$")) {
        mApplicationExtensionAreaVerticalFilter = inProcessFilter;
    } else {
        mApplicationExtensionAreaInProcessFilter = inProcessFilter;
    }
}

QRegExp mApplicationExtensionAreaOutOfProcessFilter;
void MApplicationExtensionArea::setOutOfProcessFilter(const QRegExp &outOfProcessFilter)
{
    mApplicationExtensionAreaOutOfProcessFilter = outOfProcessFilter;
}

QStringList mApplicationExtensionAreaOrder;
QStringList mApplicationExtensionAreaVerticalOrder;
void MApplicationExtensionArea::setOrder(const QStringList &order)
{
    if (order.contains("statusindicatormenu-call.desktop") || order.contains("statusindicatormenu-transfer.desktop")) {
        mApplicationExtensionAreaVerticalOrder = order;
    } else {
        mApplicationExtensionAreaOrder = order;
    }
}

QRegExp mApplicationExtensionAreaInProcessFilterDuringInit;
QRegExp mApplicationExtensionAreaVerticalFilterDuringInit;
QRegExp mApplicationExtensionAreaOutOfProcessFilterDuringInit;
QStringList mApplicationExtensionAreaOrderDuringInit;
QStringList mApplicationExtensionAreaVerticalOrderDuringInit;
bool MApplicationExtensionArea::init()
{
    mApplicationExtensionAreaInProcessFilterDuringInit = mApplicationExtensionAreaInProcessFilter;
    mApplicationExtensionAreaVerticalFilterDuringInit = mApplicationExtensionAreaVerticalFilter;
    mApplicationExtensionAreaOutOfProcessFilterDuringInit = mApplicationExtensionAreaOutOfProcessFilter;
    mApplicationExtensionAreaOrderDuringInit = mApplicationExtensionAreaOrder;
    mApplicationExtensionAreaVerticalOrderDuringInit = mApplicationExtensionAreaVerticalOrder;
    return true;
}

QList<MSceneWindow*> g_visibleSceneWindows;

void MSceneManager::appearSceneWindowNow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy)
{
    Q_UNUSED(policy);
    sceneWindow->setVisible(true);
    g_visibleSceneWindows.append(sceneWindow);
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *sceneWindow)
{
    sceneWindow->setVisible(false);
    g_visibleSceneWindows.removeAll(sceneWindow);
}

void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    sceneWindow->setVisible(false);
    g_visibleSceneWindows.removeAll(sceneWindow);
}

MSceneWindow::~MSceneWindow()
{
    g_visibleSceneWindows.removeAll(this);
}

class TestStatusIndicatorMenuExtensionInterface : public MStatusIndicatorMenuExtensionInterface
{
public:
    MStatusIndicatorMenuInterface *menuInterface;
    MWidgetController widgetController;

    TestStatusIndicatorMenuExtensionInterface() : menuInterface(NULL) {}

    bool initialize(const QString&)
    {
        return true;
    }

    void setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &mI)
    {
        menuInterface = &mI;
    }

    QGraphicsWidget* widget()
    {
        return &widgetController;
    }
};

QObject *findChildItemByObjectName(QGraphicsItem* root, const QString &name) {
    QObject* ob = dynamic_cast<QObject*>(root);

    if (ob)
        if (ob->objectName() == name)
            return ob;

    foreach(QGraphicsItem *i, root->childItems()) {
        ob = findChildItemByObjectName(i, name);

        if (ob)
            return ob;
    }

    return NULL;
}


void Ut_StatusIndicatorMenuVerticalView::init()
{
    gX11WrapperStub->stubReset();
    controller = new StatusIndicatorMenu();
    controller->setModel(new MSceneWindowModel());
    m_subject = new StatusIndicatorMenuVerticalView(controller);
    controller->setView(m_subject);
}

void Ut_StatusIndicatorMenuVerticalView::cleanup()
{
    delete controller;
}

void Ut_StatusIndicatorMenuVerticalView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatormenuverticalview";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicatorMenuVerticalView::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorMenuVerticalView::testConnections()
{
    QObject *ob = findChildItemByObjectName(controller, "StatusIndicatorMenuTopRowExtensionButton");
    QVERIFY(disconnect(ob, SIGNAL(clicked()), controller, SLOT(launchControlPanelAndHide())));

    QObject *extensionArea = findChildItemByObjectName(controller, "StatusIndicatorMenuExtensionArea");
    QVERIFY(disconnect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*))));
    QVERIFY(disconnect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), m_subject, SLOT(setExtensionLayoutPosition(MApplicationExtensionInterface*))));
}

void Ut_StatusIndicatorMenuVerticalView::testExtensionAreaInitialization()
{
    QCOMPARE(mApplicationExtensionAreaInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
    QCOMPARE(mApplicationExtensionAreaInProcessFilterDuringInit, QRegExp("/statusindicatormenu-(alarms|internetconnection|presence|profile).desktop$"));
    QCOMPARE(mApplicationExtensionAreaOutOfProcessFilterDuringInit, QRegExp("$^"));
    QCOMPARE(mApplicationExtensionAreaOrderDuringInit, ((QStringList() << "statusindicatormenu-alarms.desktop" << "statusindicatormenu-internetconnection.desktop" << "statusindicatormenu-presence.desktop" << "statusindicatormenu-profile.desktop")));
}

void Ut_StatusIndicatorMenuVerticalView::testLayoutPositions()
{
    TestStatusIndicatorMenuExtensionInterface extension;

    m_subject->setExtensionLayoutPosition(&extension);

    // All the extensions should have the vertical center position
    QCOMPARE(static_cast<MWidgetController*>(extension.widget())->layoutPosition(), M::VerticalCenterPosition);

    // Locate the settings button at the bottom of the container widget layout
    MWidget *container = static_cast<MWidget*>(findChildItemByObjectName(controller, "StatusIndicatorMenuExtensionAreaWidget"));
    MWidgetController *settingsButton = static_cast<MWidgetController*>(container->layout()->itemAt(container->layout()->count()-1));

    // It should have the bottom position set
    QCOMPARE(settingsButton->layoutPosition(), M::VerticalBottomPosition);
}

void Ut_StatusIndicatorMenuVerticalView::testCreatedItemsAreRemovedFromTheControllerAndTheScene()
{
    QList<QGraphicsItem*> itemsFromTestSubject = controller->childItems();

    controller->setView(NULL);
    m_subject = NULL;

    // All the items added by the view should have disappeared
    foreach (QGraphicsItem* item, itemsFromTestSubject) {
        QCOMPARE(controller->childItems().contains(item), QBool(false));
    }

    // All the SceneWindows should be gone as well
    QCOMPARE(g_visibleSceneWindows.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuVerticalView)
