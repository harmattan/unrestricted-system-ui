#include "callandsimapplet.h"
#include "callandsimtranslation.h"
#include "callandsimbrief.h"
#include "callandsimwidget.h"
#include "dcpcallandsim.h"

#include <DuiTheme>
#include <DuiAction>

Q_EXPORT_PLUGIN2(callandsimapplet, CallAndSimApplet)

namespace DcpCallAndSim
{
const QString css("/usr/share/duicontrolpanel/themes/style/callandsimapplet.css");
}

void CallAndSimApplet::init()
{
    DuiTheme::loadCSS(DcpCallAndSim::css);
}

DcpWidget* CallAndSimApplet::constructWidget(int widgetId)
{
    Q_UNUSED(widgetId);
    return pageMain();
}

DcpWidget* CallAndSimApplet::pageMain()
{
    if (main.isNull()) {
        main = new CallAndSimWidget();
    }

    return main;
}

QString CallAndSimApplet::title() const
{
    return DcpCallAndSim::CallAndSimText;
}

QVector<DuiAction*> CallAndSimApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    DuiAction* helpAction = new DuiAction(DcpCallAndSim::HelpText, pageMain());
    vector.append(helpAction);
    return vector;
}

DcpBrief* CallAndSimApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new CallAndSimBrief();
}

