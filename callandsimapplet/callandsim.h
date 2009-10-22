#ifndef CALLANDSIM_H
#define CALLANDSIM_H

#include <QObject>

#include <cellular-qt/CallForwarding>
#include <cellular-qt/CallWaiting>
#include <cellular-qt/SIM>

class QDBusInterface;

using namespace Cellular;

class CallAndSim : public QObject
{
    Q_OBJECT


public:
    enum Item {
        ItemCallerIdSending = 0,
        ItemCallWaiting,
        ItemCallForwarding,
        ItemPinRequest,
        ItemAll
    };

    CallAndSim(QObject* parent = 0);

signals:
    void callerIdSendingComplete(int);
    void callWaitingComplete(bool);
    void callForwardingComplete(bool, QString);
    void pinRequestComplete(bool);
    void requestFailed(CallAndSim::Item item);

public slots:
    void setCallerIdSending(int value);
    void setCallWaiting(bool enabled);
    void setCallForwarding(bool enabled, QString number);
    void setPinRequest(bool enabled);

    void changePinCode();
    void requestData(CallAndSim::Item item);

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
    SIMSecurity* simSecurity;
    QDBusInterface* dbusPinIf;
};

#endif // CALLANDSIM_H
