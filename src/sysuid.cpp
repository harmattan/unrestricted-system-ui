#include <DuiLocale>
#include <DuiTheme>

#include <QDebug>

#include "sysuid.h"

Sysuid::Sysuid(DuiApplicationWindow& window) : QObject()
{    

    qDebug() << "starting sysuidaemon";

    /* Pincode query variables */
    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("pinquery.css");
    pinCodeQueryLogic = new PinCodeQueryBusinessLogic(window);

    /* Energy UI */
    batteryLogic = new BatteryBusinessLogic();
    if(!batteryLogic->initGConfSucceeded()) {
        delete batteryLogic;
        batteryLogic = NULL;
    }

    /* Event handler */
    eventHandler = new EventHandler();

    /* Shutdown dialog */
    shutdownLogic = new ShutdownDialogBusinessLogic();
    connect(eventHandler, SIGNAL(longPowerKeyPressOccured(bool)),
            shutdownLogic, SLOT(openDialog(bool)));
    connect(eventHandler, SIGNAL(powerKeyDownOccured()),
            shutdownLogic, SLOT(powerKeyDown()));
    connect(eventHandler, SIGNAL(powerKeyUpOccured()),
            shutdownLogic, SLOT(powerKeyUp()));

    /* Lockscreen */
    lockScreenLogic = new LockScreenBusinessLogic(window);
    connect(lockScreenLogic, SIGNAL(lockScreenOff()),
            batteryLogic, SLOT(checkBattery()));
    connect(batteryLogic, SIGNAL(charging()),
            lockScreenLogic, SLOT(sleepModeOff()));
    connect(eventHandler, SIGNAL(shortPowerKeyPressOccured()),
            lockScreenLogic, SLOT(shortPowerKeyPressOccured()));
    connect(shutdownLogic, SIGNAL(dialogOpen(bool)),
            lockScreenLogic, SLOT(disable(bool)));

}

Sysuid::~Sysuid()
{
    delete pinCodeQueryLogic;
    pinCodeQueryLogic = NULL;
    delete batteryLogic;
    batteryLogic = NULL;
    delete lockScreenLogic;
    lockScreenLogic = NULL;

    delete shutdownLogic;
    shutdownLogic = NULL;
}
