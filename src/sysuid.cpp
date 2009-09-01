#include <DuiLocale>
#include <DuiTheme>

#include <QDebug>

#include "sysuid.h"

Sysuid::Sysuid() : QObject()
{    

    qDebug() << "starting sysuidaemon";

    /* Pincode query variables */
    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("pinquery.css");
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic();

    /* Energy UI */
    batteryLogic = new BatteryBusinessLogic();    

    /* Lockscreen */
    lockScreenLogic = new LockScreenBusinessLogic();
    connect(lockScreenLogic, SIGNAL(lockScreenOff()), batteryLogic, SLOT(checkBattery()));
    connect(batteryLogic, SIGNAL(charging()), lockScreenLogic, SLOT(sleepModeOff()));

    /* Event handler */
    eventHandler = new EventHandler();
    connect(eventHandler, SIGNAL(shortPowerKeyPressOccured()), lockScreenLogic, SLOT(shortPowerKeyPressOccured()));

}

Sysuid::~Sysuid()
{
//    delete pinCodeQueryLogic;
    pinCodeQueryLogic = NULL;
//    delete batteryLogic;
    batteryLogic = NULL;
    delete lockScreenLogic;
    lockScreenLogic = NULL;
}
