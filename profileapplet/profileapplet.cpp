#include "profileapplet.h"
#include "profiletranslation.h"
#include "profilewidget.h"
#include "profilebrief.h"

#include "dcpprofile.h"
#include "dcpwidget.h"

#include <QtGui>
#include <QDebug>

#include <DuiTheme>
#include <DuiAction>

Q_EXPORT_PLUGIN2(profileapplet, ProfileApplet)

const QString cssDir = "/usr/share/duicontrolpanel/themes/style/";


///////////////////////////
#include <DuiSceneManager>
void ProfileApplet::doRotation()
{
    int orientation = (int)DuiSceneManager::instance()->orientationAngle();
    orientation += 90;
    orientation %= 360;
    DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) orientation);
}
void ProfileApplet::startRotation()
{
    QTimer *rotation = new QTimer(0);
    rotation->setInterval(15*1000);
    rotation->start();
    QObject::connect(rotation, SIGNAL(timeout()), this, SLOT(doRotation()));
}
///////////////////////////

void ProfileApplet::init()
{
    DuiTheme::loadCSS(cssDir + "profileapplet.css");

    /* Un/comment this for rotation *

    startRotation();

    **/
}

DcpWidget* ProfileApplet::constructWidget(int widgetId)
{
    Q_UNUSED(widgetId);
    return pageMain();
}

DcpWidget* ProfileApplet::pageMain()
{
    qDebug() << Q_FUNC_INFO << (QObject*)main;
    if(main == NULL)
        main = new ProfileWidget();
    return main;
}

QString ProfileApplet::title() const
{
    return DcpProfile::AppletTitle;
}

QVector<DuiAction*> ProfileApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    DuiAction* helpAction = new DuiAction(DcpProfile::MenuHelpText, pageMain());
    vector.append(helpAction);
    return vector;
}

DcpBrief* ProfileApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new ProfileBrief();
}
