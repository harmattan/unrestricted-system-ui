#include "callandsimapplet.h"
#include "callandsimtranslation.h"
#include "callandsimbrief.h"
#include "callandsimwidget.h"
#include "dcpcallandsim.h"

#include <cellular-qt/NetworkRegistration>

#include <DuiTheme>
#include <DuiAction>
#include <DuiNotification>
#include <DuiInfoBanner>
#include <DuiLocale>

#include <QTimer>
#include <QDebug>
#include <QDBusInterface>
#include <QScopedPointer>

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

//    return checkNetworkState() ? pageMain() : NULL;
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
    helpAction->setLocation(DuiAction::ApplicationMenu);
    return vector;
}

DcpBrief* CallAndSimApplet::constructBrief(int partId)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(partId);
    return new CallAndSimBrief();
}

void CallAndSimApplet::switchNetworkOn()
{
    //DuiNotification* notification = new DuiNotification(QString(""), QString("summary"), QString("body"));
    //infoBanner->disappear();
}

bool CallAndSimApplet::checkNetworkState()
{
    /*
    QScopedPointer<Cellular::NetworkRegistration> nr(new Cellular::NetworkRegistration);
    QString body;
    QString buttonText;

    switch (nr->status()) {
    case Cellular::NetworkRegistration::Offline:
        body = trid("qtn_cell_error_settings_not_available",
                    "Call and SIM card settings are not available because the Phone network is off. Switch Phone network on?");
        buttonText = trid("qtn_cell_switch", "Switch");
        break;
    case Cellular::NetworkRegistration::NoSim:
        body = trid("qtn_cell_error_settings_not_sim",
                    "Call and SIM card settings are not available because there is no SIM card inserted.");
        break;
    default:
        body = "test<br><b>test</b>";
        buttonText = "test";
        break;
    }

    if (body.length() > 0) {
        DuiInfoBanner* infoBanner = new DuiInfoBanner(DuiInfoBanner::Information, "", body, "");
        if (buttonText.length() > 0) {
            infoBanner->setButtonText("qtn_cell_switch");
            connect(infoBanner, SIGNAL(buttonClicked()), this, SLOT(switchNetworkOn()));
        }
        infoBanner->appear(DuiSceneWindow::DestroyWhenDone);

        //QTimer::singleShot(3000, infoBanner, SLOT(disappear()));

        return false;
    }
*/
    return true;
}
