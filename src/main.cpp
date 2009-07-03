#include "pincodequery.h"
#include "notifier.h"
#include "businesslogic.h"
#include "main.h"

#include <QTimer>
#include <DuiSceneManager>

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiMenu>
#include <DuiNavigationBar>
#include <DuiTheme>


TestObj::TestObj() : QObject()
{
}

TestObj::~TestObj()
{
}

void TestObj::doRotation()
{
    int orientation = (int)DuiSceneManager::instance()->orientationAngle();
    orientation += 90;
    orientation %= 360;
    DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) orientation);
}

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);

    // qDebug and qWarning are filttered off.
    qInstallMsgHandler(0);


    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("pinquery.css");

    DuiApplicationWindow w;
    w.menu()->disappear();
    w.navigationBar()->disappear();
    //w.show();

    BusinessLogic logic;

    TestObj obj;
    QTimer *rotation = new QTimer(0);
    rotation->setInterval(5*1000);
    rotation->start();
    //QObject::connect(rotation, SIGNAL(timeout()), &obj, SLOT(doRotation()));

    return app.exec();
}

