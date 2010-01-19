/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <DuiLayout>
#include <DuiLabel>
#include <DuiButton>
#include <DuiLinearLayoutPolicy>

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
    DuiLinearLayoutPolicy   *landscapePolicy, *portraitPolicy;

    SYS_DEBUG ("");

    mainLayout = new DuiLayout (this);
    landscapePolicy = new DuiLinearLayoutPolicy (mainLayout, Qt::Horizontal);
    portraitPolicy  = new DuiLinearLayoutPolicy (mainLayout, Qt::Vertical);
    mainLayout->setLandscapePolicy (landscapePolicy);
    mainLayout->setPortraitPolicy (portraitPolicy);

    m_IlluminationButton = new DuiButton ();
    m_IlluminationButton->setCheckable(true);

    m_EventsButton = new DuiButton ();
    m_EventsButton->setCheckable(true);

    landscapePolicy->addItem (m_IlluminationButton, Qt::AlignRight);
    portraitPolicy->addItem (m_IlluminationButton, Qt::AlignRight);
    
    landscapePolicy->addItem (m_EventsButton, Qt::AlignRight);
    portraitPolicy->addItem (m_EventsButton, Qt::AlignRight);
}
