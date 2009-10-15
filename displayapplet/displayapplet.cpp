#include "displayapplet.h"
#include "displaywidget.h"
#include "displaytranslation.h"
#include "displaybrief.h"

#include "dcpdisplay.h"
#include "dcpwidget.h"

#include <QtGui>
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
    return new DisplayWidget();
}

QString DisplayApplet::title() const
{
    return DcpDisplay::AppletTitle;
}

QVector<DuiAction*> DisplayApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;    
    return vector;
}

DcpBrief* DisplayApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new DisplayBrief();
}
