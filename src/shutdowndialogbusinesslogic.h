#ifndef SHUTDOWNDIALOGBUSINESSLOGIC_H
#define SHUTDOWNDIALOGBUSINESSLOGIC_H

#include <QObject>
#include <QTime>

#include "shutdowndialog.h"

class DuiApplicationWindow;

class ShutdownDialogBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ShutdownDialogBusinessLogic();
    virtual ~ShutdownDialogBusinessLogic();

private: //attributes
    ShutdownDialog *shutdownDlg;
    QTimer *powerKeyPressTimer;
    QTimer *idleTimer;
    QTime t;
    bool shuttingDown;
    int currentPosInTime;
    qreal currentPos;

private: //methods
    void startPowerKeyPressTimer();
    void stopPowerKeyPressTimer();
    void stopIdleTimer();
    void setAlarmLabelText();

private slots:
    void closeDialog();
    void resetIdleTimer();
    void updateSlider();
    void shutdown();

signals:
    void dialogOpen(bool open);

public slots:
    void openDialog(bool buttonDown);
    void powerKeyDown();
    void powerKeyUp();    

};

#endif // SHUTDOWNDIALOGBUSINESSLOGIC_H
