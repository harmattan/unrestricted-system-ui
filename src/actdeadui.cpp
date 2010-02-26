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
#include <QGraphicsLinearLayout>
#include <QStringList>
#include <QTimer>

#define DEBUG
#define WARNING
#include "debug.h"

#define ANIM_TIMEOUT    500

ActDeadUI::ActDeadUI (QObject *parent) :
    QObject (parent),
    m_timer (0)
{
    SYS_DEBUG ("");
    m_logic = new ActDeadBusinessLogic (this);

    m_anim_imgs = new QStringList;
    // FIXME: Energy UI spec. contains newer image id-s,
    // but those aren't available from duitheme :-S
    m_anim_imgs->append (QString ("icon-s-battery-0"));
    m_anim_imgs->append (QString ("icon-s-battery-20"));
    m_anim_imgs->append (QString ("icon-s-battery-40"));
    m_anim_imgs->append (QString ("icon-s-battery-60"));
    m_anim_imgs->append (QString ("icon-s-battery-80"));
    m_anim_imgs->append (QString ("icon-s-battery-100"));

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
    // if (logic->getBatteryState () == QMBattery::StateCharging)
    showCharging ();
}

ActDeadUI::~ActDeadUI ()
{
    if (m_timer)
    {
        delete m_timer;
        m_timer = 0;
    }

    delete m_logic;
    m_logic = 0;
    delete m_anim_imgs;
    m_anim_imgs = 0;
}

void
ActDeadUI::createContents ()
{
    DuiLayout *layout = new DuiLayout;

    QGraphicsLinearLayout *widgets = new QGraphicsLinearLayout;

    widgets->addStretch (4);

    m_anim = new DuiImageWidget;
    m_anim->setMaximumSize (64, 64);
    widgets->addItem (m_anim);
    widgets->setAlignment (m_anim, Qt::AlignRight | Qt::AlignVCenter); 

    //% "Charging"
    m_status = new DuiLabel (qtTrId ("qtn_ener_char"));
    m_status->setAlignment (Qt::AlignLeft | Qt::AlignVCenter);
    m_status->setObjectName ("actdeadLabel");
    widgets->addItem (m_status);

    widgets->addStretch (4);

    QSize size = DuiApplication::activeApplicationWindow ()->
                 sceneManager ()->visibleSceneSize (Dui::Landscape);

    DuiGridLayoutPolicy* l_policy = new DuiGridLayoutPolicy (layout);
    l_policy->setRowFixedHeight (1, size.height ());
    l_policy->setColumnFixedWidth (0, size.width ());
    l_policy->addItem (widgets, 1, 0, Qt::AlignCenter);

    size = DuiApplication::activeApplicationWindow ()->
           sceneManager ()->visibleSceneSize (Dui::Portrait);

    DuiGridLayoutPolicy* p_policy = new DuiGridLayoutPolicy (layout);
    p_policy->setRowFixedHeight (1, size.height ());
    p_policy->setColumnFixedWidth (0, size.width ());
    p_policy->addItem (widgets, 1, 0, Qt::AlignCenter);

    DuiApplication::activeApplicationWindow ()->setAlignment (Qt::AlignCenter);

    layout->setLandscapePolicy (l_policy);
    layout->setPortraitPolicy (p_policy);

    m_window->centralWidget ()->setLayout (layout);
}

void
ActDeadUI::animate ()
{
    static int img;

    m_anim->setImage (m_anim_imgs->at (img), QSize (64, 64));

    if (++img >= m_anim_imgs->count ())
        img = 0;
}

void
ActDeadUI::chargingComplete ()
{
    SYS_DEBUG ("");

    m_anim->setVisible (false);
    // Stopping animation
    if (m_timer)
    {
        delete m_timer;
        m_timer = 0;
    }

    //% "Charging complete"
    m_status->setText (qtTrId ("qtn_ener_charcomp") +
                       QString ("<br>") +
    //% "Disconnect charger from power supply to save energy"
                       qtTrId ("qtn_ener_remcha"));

    m_logic->turnOnDisplay ();
}

void
ActDeadUI::showCharging ()
{
    SYS_DEBUG ("");

    if (m_timer == 0)
    {
        m_timer = new QTimer;
        connect (m_timer, SIGNAL (timeout ()),
                 this, SLOT (animate ()));
    }
    m_timer->start (ANIM_TIMEOUT);

    //% "Charging"
    m_status->setText (qtTrId ("qtn_ener_char"));

    // TODO: start anim
    m_logic->turnOnDisplay ();
}

void
ActDeadUI::showNotCharging ()
{
    SYS_DEBUG ("");

    // Stopping animation
    if (m_timer)
    {
        delete m_timer;
        m_timer = 0;
    }

    //% "Charging not started. Replace charger."
    m_status->setText (qtTrId ("qtn_ener_repcharger"));

    m_logic->turnOnDisplay ();
}

void
ActDeadUI::powersave (bool active)
{
    if (m_timer == 0)
        return;

    // Do not animate when screen is off
    if (active)
        m_timer->start ();
    else
        m_timer->stop ();
}

