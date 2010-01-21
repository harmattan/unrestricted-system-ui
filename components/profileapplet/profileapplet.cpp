/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "profileapplet.h"
#include "profilewidget.h"
#include "profilebrief.h"

#include "dcpprofile.h"
#include "dcpwidget.h"

#include <QtGui>
#include <QDebug>

#include <DuiTheme>
#include <DuiAction>

Q_EXPORT_PLUGIN2(profileapplet, ProfileApplet)

const QString cssDir = "/usr/share/themes/base/dui/duicontrolpanel/style/";


void ProfileApplet::init()
{
    DuiTheme::loadCSS(cssDir + "profileapplet.css");
}

DcpWidget* ProfileApplet::constructWidget(int widgetId)
{
    Q_UNUSED(widgetId);
    return pageMain();
}

DcpWidget* ProfileApplet::pageMain()
{
    qDebug() << Q_FUNC_INFO << (QObject*)main;
    if (main == NULL)
        main = new ProfileWidget();
    return main;
}

QString ProfileApplet::title() const
{
    //% "Profiles"
    return qtTrId ("qtn_prof_profile");
}

QVector<DuiAction*> ProfileApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    //% "Help"
    DuiAction* helpAction = new DuiAction (qtTrId ("qtn_comm_help"), pageMain ());
    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenuLocation);
    return vector;
}

DcpBrief* ProfileApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new ProfileBrief();
}
