/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "actdeadui.h"

#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiSceneManager>
#include <DuiImageWidget>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>

#define DEBUG
#define WARNING
#include "debug.h"

ActDeadUI::ActDeadUI (QObject *parent) :
    QObject (parent)
{
    SYS_DEBUG ("");
    m_logic = new ActDeadBusinessLogic (this);
    m_window = new DuiApplicationPage;
    m_window->setObjectName ("actdeadWindow");

    m_window->setPannableAreaInteractive (false);
    // Its just for testing on top of the running desktop...
    m_window->setComponentsDisplayMode (DuiApplicationPage::AllComponents,
                                        DuiApplicationPageModel::Hide);
    // Create the main widgets (and show the main window before
    // for correct size calculations..)
    DuiApplication::activeApplicationWindow()->show();
    createContents ();
    // Show the application page
    m_window->appearNow ();

    // TODO: Check whether the charging really happening...
    showCharging ();
}

ActDeadUI::~ActDeadUI ()
{
    delete m_logic;
    m_logic = 0;
    delete m_window;
    m_window = 0;
}

void
ActDeadUI::createContents ()
{
    DuiLayout *layout = new DuiLayout;
// XXX Create the widgets here..
    DuiLabel *widget = new DuiLabel ("Charging...");
    widget->setAlignment (Qt::AlignCenter);
    widget->setObjectName ("actdeadLabel");

    QSize size = DuiApplication::activeApplicationWindow ()->
                 sceneManager ()->visibleSceneSize (Dui::Landscape);

    DuiGridLayoutPolicy* l_policy = new DuiGridLayoutPolicy (layout);
    l_policy->setSpacing (10);
    l_policy->setRowFixedHeight (1, size.height ());
    l_policy->setColumnFixedWidth (0, size.width ());
    l_policy->addItem (widget, 1, 0, Qt::AlignCenter);

    size = DuiApplication::activeApplicationWindow ()->
           sceneManager ()->visibleSceneSize (Dui::Portrait);

    DuiGridLayoutPolicy* p_policy = new DuiGridLayoutPolicy (layout);
    p_policy->setSpacing (10);
    p_policy->setRowFixedHeight (1, size.height ());
    p_policy->setColumnFixedWidth (0, size.width ());
    p_policy->addItem (widget, 1, 0, Qt::AlignCenter);

    layout->setLandscapePolicy (l_policy);
    layout->setPortraitPolicy (p_policy);

    m_window->centralWidget ()->setLayout (layout);
}

void
ActDeadUI::chargingComplete ()
{
    SYS_DEBUG ("");
    // TODO
    // Show "Unplug the charger from wall connector" message on the window
    m_logic->turnOnDisplay ();
}

void
ActDeadUI::showCharging ()
{
    SYS_DEBUG ("");
    // TODO
    m_logic->turnOnDisplay ();
}

void
ActDeadUI::showNotCharging ()
{
    SYS_DEBUG ("");
    // TODO
    m_logic->turnOnDisplay ();
}

void
ActDeadUI::batteryEvent (QmBattery::State)
{
    // XXX:
    // show not charging when applicable
}

