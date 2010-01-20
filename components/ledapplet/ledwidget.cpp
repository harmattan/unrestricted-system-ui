/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <DuiLayout>
#include <DuiLabel>
#include <DuiButton>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>

#include "ledwidget.h"

#define DEBUG
#include "../debug.h"

LedWidget::LedWidget (
        QGraphicsWidget *parent):
    DcpWidget (parent)
{
    SYS_DEBUG ("");
    initWidget ();
}

/*!
 * Ok, this widget is not even half ready, I have to on some other stuff now...
 */
void
LedWidget::initWidget ()
{
    DuiLayout               *mainLayout;
    DuiGridLayoutPolicy     *landscapePolicy;
    DuiLinearLayoutPolicy   *portraitPolicy;
    DuiLabel                *label1, *label2;

    SYS_DEBUG ("");

    mainLayout = new DuiLayout (this);
    landscapePolicy = new DuiGridLayoutPolicy (mainLayout);
    portraitPolicy  = new DuiLinearLayoutPolicy (mainLayout, Qt::Vertical);
    mainLayout->setLandscapePolicy (landscapePolicy);
    mainLayout->setPortraitPolicy (portraitPolicy);

    label1 = new DuiLabel ("Illumination light");

    m_IlluminationButton = new DuiButton ();
    m_IlluminationButton->setCheckable (true);

    label2 = new DuiLabel ("Incoming events");

    m_EventsButton = new DuiButton ();
    m_EventsButton->setCheckable (true);

    /*
     * Well, no UI spec yet, so I'm not sure this is what we want...
     */
    portraitPolicy->addItem (label1, Qt::AlignLeft);
    portraitPolicy->addItem (m_IlluminationButton, Qt::AlignRight);
    portraitPolicy->addItem (label2, Qt::AlignLeft);
    portraitPolicy->addItem (m_EventsButton, Qt::AlignRight);
   
    landscapePolicy->addItemAtPosition (label1, 0, 0);
    landscapePolicy->addItemAtPosition (m_IlluminationButton, 0, 1);
    landscapePolicy->addItemAtPosition (label2, 1, 0);
    landscapePolicy->addItemAtPosition (m_EventsButton, 1, 1);

    connect (m_IlluminationButton, SIGNAL (toggled(bool)),
            this, SLOT (illuminationToggled(bool)));
    connect (m_EventsButton, SIGNAL (toggled(bool)),
            this, SLOT (eventsToggled(bool)));
}

void 
LedWidget::illuminationToggled (
        bool newState)
{
    SYS_DEBUG ("*** state = %s", newState ? "true" : "false");
}

void 
LedWidget::eventsToggled (
        bool newState)
{
    SYS_DEBUG ("*** state = %s", newState ? "true" : "false");
}
