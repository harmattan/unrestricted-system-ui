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
    switch (widgetId) {
        case DcpBattery::Main:
            return pageMain();
        break;
        default:
            qDebug() << "Page Unknown";
            return 0;
        break;
    }
}

DcpWidget* BatteryApplet::pageMain()
{
    return new BatteryWidget();
}

QString BatteryApplet::title() const
{
    return DcpBattery::AppletTitle;
}

QVector<DuiAction*> BatteryApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    return vector;
}

DcpBrief* BatteryApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new BatteryBrief();
}
