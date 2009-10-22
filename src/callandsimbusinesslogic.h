#ifndef CALLANDSIMBUSINESSLOGIC_H
#define CALLANDSIMBUSINESSLOGIC_H

#include <QObject>

#include <CallWaiting>
#include <CallForwarding>
#include <SIM>

class QString;

using namespace Cellular;

class CallAndSimBusinessLogic : public QObject
{
    Q_OBJECT

public:
    CallAndSimBusinessLogic();

private slots:
    void waitingActivateComplete(CallWaiting::WaitingError);
    void waitingCancelComplete(CallWaiting::WaitingError);
    void waitingCheckComplete(bool active, CallWaiting::WaitingError);

    void divertActivateComplete(CallForwarding::DivertError error);
    void divertCancelComplete(CallForwarding::DivertError error);
    void divertCheckComplete(bool active, QString number, CallForwarding::DivertError error);

signals:
    void callerIdSending(int value);
    void callWaiting(bool enabled);
    void callForwarding(bool enabled);
    void forwardTo(QString number);
    void pinRequest(bool enabled);

public:
    void getCallerIdSending();
    void getCallWaiting();
    void getCallForwarding();
    void getForwardTo();
    void getPinRequest();

    void setCallerIdSending(int value);
    void setCallWaiting(bool enable);
    void setCallForwarding(bool enable);
    void setForwardTo(const QString& number);
    void setPinRequest(bool enable);

private:
    int i1;
    bool b2, b3;
    QString s1;

    CallForwarding* callForwarding;
    CallWaiting* callWaiting;
    SIMSecurity *simSecurity;
};

#endif // CALLANDSIMBUSINESSLOGIC_H
