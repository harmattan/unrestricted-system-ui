#ifndef CALLANDSIM_H
#define CALLANDSIM_H

#include "dcpcallandsim.h"

#include <cellular-qt/SIM>
//#include <cellular-qt/NetworkRegistration>

#include <QObject>

class QDBusInterface;

using namespace Cellular;

class CallAndSim : public QObject
{
    Q_OBJECT

public:
    CallAndSim(QObject* parent = 0);

signals:
    void callerIdSendingComplete(int);
    void pinRequestComplete(bool);

public slots:
    void setPinRequest(bool enabled);
    void changePinCode();

private slots:

    void pinQueryStateComplete(SIMSecurity::PINQuery state, SIMError error = SIMErrorNone);
    void pinQueryEnabled(SIMSecurity::PINQuery queryState);

    void networkStatusChanges(int status);

    void DBusMessagingFailure();

private:
    SIMSecurity* simSecurity;
    //NetworkRegistration *networkRegistration;
    QDBusInterface* dbusPinIf;
};

#endif // CALLANDSIM_H
