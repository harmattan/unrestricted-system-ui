#ifndef SHUTDOWNBUSINESSLOGIC_H
#define SHUTDOWNBUSINESSLOGIC_H

#include <QObject>
#include <QTime>

#include "shutdownui.h"

class DuiApplicationWindow;

class ShutdownBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ShutdownBusinessLogic(QObject* parent = 0);
    virtual ~ShutdownBusinessLogic();

private: //methods
    void startPowerKeyPressTimer();
    void stopPowerKeyPressTimer();
    void stopIdleTimer();

private slots:
    void closeDialog();
    void resetIdleTimer();
    void shutdown();

signals:
    void dialogOpen(bool open);

public slots:
    void openDialog(bool buttonDown);
    void powerKeyDown();
    void powerKeyUp();    

    private: //attributes
    ShutdownUI* ui;
    QTimer *powerKeyPressTimer;
    QTimer *idleTimer;
    QTime t;
    bool shuttingDown;
};

#endif // SHUTDOWNBUSINESSLOGIC_H
