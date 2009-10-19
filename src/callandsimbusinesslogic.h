#ifndef CALLANDSIMBUSINESSLOGIC_H
#define CALLANDSIMBUSINESSLOGIC_H

#include <QObject>

class QString;

class CallAndSimBusinessLogic : public QObject
{
    Q_OBJECT

public:
    CallAndSimBusinessLogic();
    virtual ~CallAndSimBusinessLogic();

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
};

#endif // CALLANDSIMBUSINESSLOGIC_H
