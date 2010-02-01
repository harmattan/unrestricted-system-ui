/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <QtGui>
#include <QDebug>
#include <DuiTheme>
#include <DuiAction>

#include "ledbrief.h"
#include "ledapplet.h"
#include "ledwidget.h"
#include "leddbusinterface.h"

//#define DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(ledapplet, LedApplet)

const QString cssDir = "/usr/share/themes/base/dui/duicontrolpanel/style/";


LedApplet::LedApplet ()
{
   m_LedDBusInterface = new  LedDBusInterface ();
}

LedApplet::~LedApplet () 
{
    delete m_LedDBusInterface;
    m_LedDBusInterface = 0;
}


void 
LedApplet::init ()
{
    DuiTheme::loadCSS (cssDir + "ledapplet.css");
}

DcpWidget *
LedApplet::constructWidget (
        int widgetId)
{
    Q_UNUSED (widgetId);

    SYS_DEBUG ("Contructing LedWidget");
    return new LedWidget (m_LedDBusInterface);
}

QString
LedApplet::title() const
{
    //% "???"
    return "Power Key Light"; //qtTrId ("qtn_prof_profile");
}

QVector<DuiAction*> 
LedApplet::viewMenuItems ()
{
    QVector<DuiAction*> vector;
#if 0
    //% "Help"
    DuiAction* helpAction = new DuiAction (qtTrId ("qtn_comm_help"), pageMain ());
    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenuLocation);
#endif
    return vector;
}

DcpBrief *
LedApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new LedBrief (m_LedDBusInterface);
}
