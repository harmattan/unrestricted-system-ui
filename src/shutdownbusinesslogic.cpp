#include <QDebug>
#include <QTimer>
#include <DuiApplicationWindow>
#include <DuiApplication>

#include "shutdownbusinesslogic.h"
#include <qmsystem/qmsystemstate.h>
#include <qmsystem/qmled.h>

const int SHUTDOWN_TIME = 2000/*ms*/;
const int UPDATE_INTERVAL = 50;/*ms*/
const int IDLE_TIME = 10000/*ms*/;

using namespace Maemo;

ShutdownBusinessLogic::ShutdownBusinessLogic() :
        QObject(),
        ui(NULL),
        powerKeyPressTimer(NULL),
        idleTimer(NULL),
        shuttingDown(false)
{
}

ShutdownBusinessLogic::~ShutdownBusinessLogic()
{
    if (ui) {
        ui->deleteLater();
        ui = NULL;
    }
}

void ShutdownBusinessLogic::openDialog(bool btnDown)
{
    qDebug() << "openDialog";
/*    if (shutdownDlg != NULL || shuttingDown) {
        qDebug() << "won't open shuttingDown:" << shuttingDown;
        return;
    }*/
    emit dialogOpen(true);

    DuiApplication::instance()->applicationWindow()->show();

//    shutdownDlg = new ShutdownDialog(QString("Shutdown dialog"),
//                                     QString("next alarm event"), DuiDialog::NoButton);

    if (btnDown)
        startPowerKeyPressTimer();        

    resetIdleTimer();

    /* shutdownDlg->setSystemModal(); NOT YET IMPLEMENTED */
//    shutdownDlg->exec();
}

void ShutdownBusinessLogic::closeDialog()
{
    /* Here we should turn off the device instead of just deleting the dialog */
    qDebug() << "closeDialog()";
    stopPowerKeyPressTimer();
    stopIdleTimer();
/*    if (shutdownDlg) {
        shutdownDlg->reject();
        shutdownDlg->deleteLater();
        shutdownDlg = NULL;
    }*/
    DuiApplication::instance()->applicationWindow()->hide();
    emit dialogOpen(false);
}

void ShutdownBusinessLogic::powerKeyDown()
{       
    qDebug() << "powerKeyDown()";
//    if(shuttingDown || shutdownDlg == NULL)
//        return;

    resetIdleTimer();

    if(powerKeyPressTimer == NULL)
        startPowerKeyPressTimer();
}

void ShutdownBusinessLogic::powerKeyUp()
{
    qDebug() << "powerKeyUp()";
//    if(shuttingDown || shutdownDlg == NULL)
//        return;

    stopPowerKeyPressTimer();
}

void ShutdownBusinessLogic::startPowerKeyPressTimer()
{
    qDebug() << "startPowerKeyPressTimer()";
    t.start();

    if(powerKeyPressTimer == NULL) {
        powerKeyPressTimer = new QTimer(this);
        powerKeyPressTimer->setInterval(UPDATE_INTERVAL);
        powerKeyPressTimer->start();
    }
}

void ShutdownBusinessLogic::stopPowerKeyPressTimer()
{   
    if(powerKeyPressTimer != NULL) { 
        powerKeyPressTimer->stop();
        powerKeyPressTimer->deleteLater();        
        powerKeyPressTimer = NULL;        
    }
}

void ShutdownBusinessLogic::stopIdleTimer()
{
    qDebug() << "stopIdleTimer()";
    if(idleTimer != NULL) {
        idleTimer->stop();
        idleTimer->deleteLater();
        idleTimer = NULL;
    }
}

void ShutdownBusinessLogic::resetIdleTimer()
{
    if(shuttingDown)
        return;

    if(idleTimer == NULL) {
        idleTimer = new QTimer(this);
        connect(idleTimer, SIGNAL(timeout()), this, SLOT(closeDialog()));
    }
    idleTimer->start(IDLE_TIME);
}

void ShutdownBusinessLogic::shutdown()
{
    qDebug() << "shutdown()";
    stopPowerKeyPressTimer();
    stopIdleTimer();
    shuttingDown = true;

    QmLED led;
    led.activate(QString("PatternShutDown"));

    QmSystemState state;
    state.set(QmSystemState::Shutdown);        
}
