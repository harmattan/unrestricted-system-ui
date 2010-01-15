#include "batteryapplet.h"
#include "batterywidget.h"
#include "batterybrief.h"

#include "dcpbattery.h"

#include <dcpwidget.h>

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
    if (main == NULL)
        main = new BatteryWidget();
    return main;
}

QString BatteryApplet::title() const
{
    //% "Battery"
    return qtTrId ("qtn_ener_battery");
}

QVector<DuiAction*> BatteryApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    //% "Help"
    DuiAction* helpAction = new DuiAction(qtTrId ("qtn_comm_help"), pageMain());
    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenu);
    return vector;
}

DcpBrief* BatteryApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new BatteryBrief();
}

