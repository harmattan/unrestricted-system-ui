#include "networkapplet.h"
#include "networkwidget.h"
#include "networktranslation.h"
#include "networkbrief.h"

#include "dcpnetwork.h"
#include "dcpwidget.h"

#include <QtGui>
#include <QDebug>

#include <DuiTheme>
#include <DuiAction>

Q_EXPORT_PLUGIN2(networkapplet, NetworkApplet)

const QString cssDir = "/usr/share/duicontrolpanel/themes/style/";

void NetworkApplet::init()
{    
    DuiTheme::loadCSS(cssDir + "networkapplet.css");
};

DcpWidget* NetworkApplet::constructWidget(int widgetId)
{ 
    Q_UNUSED(widgetId);
    return pageMain();
}

DcpWidget* NetworkApplet::pageMain()
{    
     if(main == NULL)
        main = new NetworkWidget();
    return main;    
}

QString NetworkApplet::title() const
{
    return DcpNetwork::PhoneNetworkText;
}

QVector<DuiAction*> NetworkApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    DuiAction* helpAction = new DuiAction(DcpNetwork::HelpText, pageMain());
    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenu);
    return vector;
}

DcpBrief* NetworkApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new NetworkBrief();
}
