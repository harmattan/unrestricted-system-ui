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

    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("pinquery.css");

    DuiApplicationWindow w;
    w.menu()->disappear();
    w.navigationBar()->disappear();
    w.show();

    Notifier *notifier = NULL;
    notifier = new Notifier();
    notifier->showNotification(Notifier::SIMCardInserted);

    PinCodeQuery *pinCodeQuery = NULL;
    pinCodeQuery = new PinCodeQuery(notifier);
    //pinCodeQuery->appearNow();

    BusinessLogic logic(pinCodeQuery, notifier);

    TestObj obj;
    QTimer *rotation = new QTimer(0);
    rotation->setInterval(3*1000);
    rotation->start();
    //QObject::connect(rotation, SIGNAL(timeout()), &obj, SLOT(doRotation()));

    return app.exec();
}

