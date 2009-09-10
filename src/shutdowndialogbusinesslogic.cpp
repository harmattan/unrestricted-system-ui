#include <QDebug>
#include <QTimer>

#include "shutdowndialogbusinesslogic.h"

const int SHUTDOWN_TIME = 8000/*ms*/;
const int UPDATE_INTERVAL = 50;/*ms*/
const int IDLE_TIME = 10000/*ms*/;
const int SLIDER_RANGE = 1000;

ShutdownDialogBusinessLogic::ShutdownDialogBusinessLogic()
{
    shutdownDlg = NULL;    
}
ShutdownDialogBusinessLogic::~ShutdownDialogBusinessLogic()
{
    if (shutdownDlg)
        shutdownDlg->deleteLater();
    shutdownDlg = NULL;
}

void ShutdownDialogBusinessLogic::openDialog(bool btnDown)
{
    qDebug() << "openDialog";
    if (shutdownDlg != NULL) {
        qDebug() << "won't open";
        return;
    }
    emit dialogOpen(true);

    powerKeyPressTimer = NULL;
    idleTimer = NULL;

    shutdownDlg = new ShutdownDialog(QString("Shutdown dialog"), QString("next alarm event"), DuiDialog::NoButton, SLIDER_RANGE);
    connect(shutdownDlg->slider(), SIGNAL(unlocked()), this, SLOT(closeDialog()));
    connect(shutdownDlg->slider(), SIGNAL(valueChanged()), this, SLOT(resetIdleTimer()));

    if(btnDown)
        startPowerKeyPressTimer();        

    resetIdleTimer();

    /* shutdownDlg->setSystemModal(); NOT YET IMPLEMENTED */
    shutdownDlg->exec();

}

void ShutdownDialogBusinessLogic::closeDialog()
{
    /* Here we should turn off the device instead of just deleting the dialog */

    qDebug() << "closeDialog()";
    stopPowerKeyPressTimer();
    stopIdleTimer();
    shutdownDlg->accept();
    shutdownDlg->deleteLater();
    shutdownDlg = NULL;
    emit dialogOpen(false);    
}

void ShutdownDialogBusinessLogic::powerKeyDown()
{       
    qDebug() << "powerKeyDown()";
    if(powerKeyPressTimer == NULL)
        startPowerKeyPressTimer();
    if(shutdownDlg != NULL)
        if(shutdownDlg->slider() != NULL)
            shutdownDlg->slider()->hwKeyDown(true);
}

void ShutdownDialogBusinessLogic::powerKeyUp()
{    
    qDebug() << "powerKeyUp()";
    stopPowerKeyPressTimer();
    if(shutdownDlg != NULL)
        if(shutdownDlg->slider() != NULL)
            shutdownDlg->slider()->hwKeyDown(false);
}

void ShutdownDialogBusinessLogic::startPowerKeyPressTimer()
{
    qDebug() << "startPowerKeyPressTimer()";
    t.start();

    if(powerKeyPressTimer == NULL) {
        powerKeyPressTimer = new QTimer(this);
        powerKeyPressTimer->setInterval(UPDATE_INTERVAL);
        connect(powerKeyPressTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));
        powerKeyPressTimer->start();
    }
}

void ShutdownDialogBusinessLogic::stopPowerKeyPressTimer()
{   
    if(powerKeyPressTimer != NULL) { 
        powerKeyPressTimer->stop();
        powerKeyPressTimer->deleteLater();        
        powerKeyPressTimer = NULL;        
    }
}

void ShutdownDialogBusinessLogic::stopIdleTimer()
{
    if(idleTimer != NULL) {
        idleTimer->stop();
        idleTimer->deleteLater();
        idleTimer = NULL;
    }
}

void ShutdownDialogBusinessLogic::resetIdleTimer()
{
    if(idleTimer == NULL) {
        idleTimer = new QTimer(this);
        connect(idleTimer, SIGNAL(timeout()), this, SLOT(closeDialog()));
    }
    idleTimer->start(IDLE_TIME);
}

void ShutdownDialogBusinessLogic::updateSlider()
{
    qDebug() << "updateSlider";
    if(shutdownDlg != NULL)
        if(shutdownDlg->slider() != NULL)
            shutdownDlg->slider()->updateValue((int)(SLIDER_RANGE * (double)t.elapsed()/SHUTDOWN_TIME));

    /* Note: We cannot just use the powerKeyPressTimer and check when it reaches the SHUTDOWN_TIME.
       Updating the slider takes time so that the powerKeyPressTimer reaches the SHUTDOWN_TIME way too
       late. Instead we use separate QTime t to define the actual SHUTDOWN_TIME. */
}
