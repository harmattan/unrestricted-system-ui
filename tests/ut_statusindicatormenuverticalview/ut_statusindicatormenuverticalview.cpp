
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
#include <QGraphicsLinearLayout>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include "x11wrapper_modified_stub.h"

#ifdef HAVE_QMSYSTEM
#include "qmlocks_stub.h"
#endif

// MApplicationExtensionArea stubs
MApplicationExtensionArea *mApplicationExtensionAreaInstance = NULL;
QString mApplicationExtensionAreaInterface;
MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, QGraphicsItem *)
{
    mApplicationExtensionAreaInstance = this;
    mApplicationExtensionAreaInterface = interface;
}

MApplicationExtensionArea::~MApplicationExtensionArea()
{
    mApplicationExtensionAreaInstance = NULL;
}

QRegExp mApplicationExtensionAreaInProcessFilter;
void MApplicationExtensionArea::setInProcessFilter(const QRegExp &inProcessFilter)
{
    mApplicationExtensionAreaInProcessFilter = inProcessFilter;
}

QRegExp mApplicationExtensionAreaOutOfProcessFilter;
void MApplicationExtensionArea::setOutOfProcessFilter(const QRegExp &outOfProcessFilter)
{
    mApplicationExtensionAreaOutOfProcessFilter = outOfProcessFilter;
}

QStringList mApplicationExtensionAreaOrder;
void MApplicationExtensionArea::setOrder(const QStringList &order)
{
    mApplicationExtensionAreaOrder = order;
}

QRegExp mApplicationExtensionAreaInProcessFilterDuringInit;
QRegExp mApplicationExtensionAreaOutOfProcessFilterDuringInit;
QStringList mApplicationExtensionAreaOrderDuringInit;
bool MApplicationExtensionArea::init()
{
    mApplicationExtensionAreaInProcessFilterDuringInit = mApplicationExtensionAreaInProcessFilter;
    mApplicationExtensionAreaOutOfProcessFilterDuringInit = mApplicationExtensionAreaOutOfProcessFilter;
    mApplicationExtensionAreaOrderDuringInit = mApplicationExtensionAreaOrder;
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
    mApplicationExtensionAreaInstance = NULL;
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

MWidget *getContainerFromLayout(MWidgetController *controller)
{
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(controller->layout());
    if (layout != NULL && layout->count() > 0) {
        return static_cast<MWidget *>(layout->itemAt(layout->count() - 1));
    } else {
        return NULL;
    }
}

void Ut_StatusIndicatorMenuVerticalView::testConnections()
{
    MWidget *container = getContainerFromLayout(controller);
    QVERIFY(container != NULL);

    QVERIFY(disconnect(mApplicationExtensionAreaInstance, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*))));
    QVERIFY(disconnect(mApplicationExtensionAreaInstance, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), m_subject, SLOT(setExtensionLayoutPosition(MApplicationExtensionInterface*))));
}

void Ut_StatusIndicatorMenuVerticalView::testExtensionAreaInitialization()
{
    QCOMPARE(mApplicationExtensionAreaInterface, QString("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0"));
    QCOMPARE(mApplicationExtensionAreaInProcessFilterDuringInit,
             QRegExp("/statusindicatormenu-(volume|call|internetconnection|bluetooth|presence|transfer).desktop$"));
    QCOMPARE(mApplicationExtensionAreaOutOfProcessFilterDuringInit, QRegExp("$^"));
    QCOMPARE(mApplicationExtensionAreaOrderDuringInit, (QStringList() << "statusindicatormenu-volume.desktop"
                             << "statusindicatormenu-call.desktop"
                             << "statusindicatormenu-internetconnection.desktop"
                             << "statusindicatormenu-bluetooth.desktop"
                             << "statusindicatormenu-presence.desktop"
                             << "statusindicatormenu-transfer.desktop"));
}

void Ut_StatusIndicatorMenuVerticalView::testLayoutPositions()
{
    TestStatusIndicatorMenuExtensionInterface extension;

    m_subject->setExtensionLayoutPosition(&extension);

    // All the extensions should have the vertical center position
    QCOMPARE(static_cast<MWidgetController*>(extension.widget())->layoutPosition(), M::VerticalCenterPosition);

    // Locate the divider at the bottom of the container widget layout
    MWidget *container = getContainerFromLayout(controller);
    MWidgetController *divider = static_cast<MWidgetController*>(container->layout()->itemAt(container->layout()->count()-1));

    // It should have the bottom position set
    QCOMPARE(divider->layoutPosition(), M::VerticalTopPosition);
}

void Ut_StatusIndicatorMenuVerticalView::testCreatedItemsAreRemovedFromTheControllerAndTheScene()
{
    QSKIP("This test is disabled due to bug NB#217876. It's not possible to know which children are created by the view being tested.", SkipSingle);

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
