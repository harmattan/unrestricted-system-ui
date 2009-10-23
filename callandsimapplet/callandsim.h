#ifndef CALLANDSIM_H
#define CALLANDSIM_H

#include "dcpcallandsim.h"

#include <cellular-qt/CallForwarding>
#include <cellular-qt/CallWaiting>
#include <cellular-qt/SIM>

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
    void callWaitingComplete(bool);
    void callForwardingComplete(bool, QString);
    void pinRequestComplete(bool);
    void requestFailed(DcpCallAndSim::Data data);

public slots:
    void setCallerIdSending(int value);
    void setCallWaiting(bool enabled);
    void setCallForwarding(bool enabled, QString number);
    void setPinRequest(bool enabled);

    void changePinCode();
    void requestData(DcpCallAndSim::Data data);

private slots:
    void waitingActivateComplete(CallWaiting::WaitingError);
    void waitingCancelComplete(CallWaiting::WaitingError);
    void waitingCheckComplete(bool active, CallWaiting::WaitingError);

    void divertActivateComplete(CallForwarding::DivertError error);
    void divertCancelComplete(CallForwarding::DivertError error);
    void divertCheckComplete(bool active, QString number, CallForwarding::DivertError error);

private:
    CallForwarding* callForwarding;
    CallWaiting* callWaiting;
    QDBusInterface* dbusPinIf;
};

#endif // CALLANDSIM_H
