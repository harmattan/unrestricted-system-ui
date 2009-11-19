#ifndef SHUTDOWNBUSINESSLOGIC_H
#define SHUTDOWNBUSINESSLOGIC_H

#include "notifier.h"

#include <qmsystem/qmsystemstate.h>

#include <QObject>

class ShutdownUI;

class ShutdownBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ShutdownBusinessLogic(QObject* parent = 0);
    virtual ~ShutdownBusinessLogic();

public slots:
    void systemStateChanged(Maemo::QmSystemState::StateIndication what);

signals:
    void showNotification(const QString& notifText, NotificationType::Type type);

private:
    void normalShutdown();
    void thermalShutdown();
    void batteryShutdown();
    void shutdownDeniedUSB();

private:
    ShutdownUI* ui;
    Maemo::QmSystemState* state;
};

#endif // SHUTDOWNBUSINESSLOGIC_H
