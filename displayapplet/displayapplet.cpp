#include "displayapplet.h"
#include "displaywidget.h"
#include "displaybrief.h"

#include "dcpdisplay.h"
#include <dcpwidget.h>

#include <QtGui>
#include <QtPlugin>
#include <QDebug>

#include <DuiTheme>
#include <DuiAction>

Q_EXPORT_PLUGIN2(displayapplet, DisplayApplet)

const QString cssDir = "/usr/share/duicontrolpanel/themes/style/";

void DisplayApplet::init()
{
    DuiTheme::loadCSS(cssDir + "displayapplet.css");
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
    helpAction->setLocation(DuiAction::ApplicationMenu);
    return vector;
}

DcpBrief* DisplayApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new DisplayBrief();
}
