/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displayapplet.h"
#include "displaywidget.h"
#include "displaybrief.h"

#include "dcpdisplay.h"
#include <dcpwidget.h>
#include <dcpwidgettypes.h>


#include <QtGui>
#include <QtPlugin>
#include <QDebug>

#include <DuiTheme>
#include <DuiAction>

#define DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(displayapplet, DisplayApplet)

const QString cssDir = "/usr/share/themes/base/dui/duicontrolpanel/style/";

void 
DisplayApplet::init()
{
    SYS_DEBUG ("");
    DuiTheme::loadCSS (cssDir + "displayapplet.css");
}

DcpWidget* DisplayApplet::constructWidget(int widgetId)
{
    Q_UNUSED(widgetId);
    return pageMain();
}

DcpWidget* DisplayApplet::pageMain()
{
    if (main == NULL)
        main = new DisplayWidget();
    return main;
}

QString DisplayApplet::title() const
{
    //% "Display"
    return qtTrId ("qtn_dcp_display_title");
}

QVector<DuiAction*> DisplayApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    //% "Help"
    DuiAction* helpAction = new DuiAction(qtTrId ("qtn_comm_help"), pageMain());
    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenuLocation);
    return vector;
}

DcpBrief* DisplayApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new DisplayBrief();
}
