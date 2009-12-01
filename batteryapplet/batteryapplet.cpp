#include "batteryapplet.h"
#include "batterywidget.h"
#include "batterytranslation.h"
#include "batterybrief.h"

#include "dcpbattery.h"
#include "dcpwidget.h"

#include <QtGui>
#include <QDebug>

#include <DuiTheme>
#include <DuiAction>

Q_EXPORT_PLUGIN2(batteryapplet, BatteryApplet)

const QString cssDir = "/usr/share/duicontrolpanel/themes/style/";

void BatteryApplet::init()
{   
    DuiTheme::loadCSS(cssDir + "batteryapplet.css");    
}

DcpWidget* BatteryApplet::constructWidget(int widgetId)
{
    Q_UNUSED(widgetId);
    return pageMain();    
}

DcpWidget* BatteryApplet::pageMain()
{
    if(main == NULL)
        main = new BatteryWidget();
    return main;
}

QString BatteryApplet::title() const
{
    return DcpBattery::AppletTitle;
}

QVector<DuiAction*> BatteryApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    DuiAction* helpAction = new DuiAction(DcpNetwork::HelpText, pageMain());
    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenu);
    return vector;
}

DcpBrief* BatteryApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new BatteryBrief();
}
