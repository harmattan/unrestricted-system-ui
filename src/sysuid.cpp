#include <DuiLocale>
#include <DuiTheme>

#include "sysuid.h"

Sysuid::Sysuid() : QObject()
{    
    qDebug() << "starting sysuidaemon";

    /* Pincode query variables */
    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("pinquery.css");
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic();

    /* Battery variables */
    //batteryLogic = new BatteryBusinessLogic(); //not yet working
}

Sysuid::~Sysuid()
{
    delete pinCodeQueryLogic;
    pinCodeQueryLogic = NULL;
    /*
    delete batteryLogic;
    batteryLogic = NULL;
    */
}
