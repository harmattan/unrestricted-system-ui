#ifndef CALLANDSIMDBUSINTERFACE_H
#define CALLANDSIMDBUSINTERFACE_H

#include <QObject>

class QDBusInterface;

class CallAndSimDBusInterface : public QObject
{
public:
    enum CallerIdSending {
        NetworkChoose = 0,
        Yes,
        No
    };

    CallAndSimDBusInterface();
    virtual ~CallAndSimDBusInterface();

    void callerIdSendingRequired();
    void callWaitingRequired();
    void callForwardingRequired();
    void forwardToRequired();
    void pinRequestRequired();

    void setCallerIdSending(CallerIdSending value);
    void setCallWaiting(bool enabled);
    void setCallForwarding(bool enabled);
    void setForwardTo(const QString& number);
    void setPinRequest(bool enabled);

private slots:
    void valueSet();
    void DBusMessagingFailure();

signals:
    void callerIdSending(CallerIdSending);
    void callWaiting(bool);
    void callForwarding(bool);
    void forwardTo(QString);
    void pinRequest(bool);

private:
    QDBusInterface *dbusIf;
};

#endif // CALLANDSIMDBUSINTERFACE_H
