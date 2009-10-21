#ifndef CALLANDSIMDBUSINTERFACE_H
#define CALLANDSIMDBUSINTERFACE_H

#include <QObject>

class QDBusInterface;

class CallAndSimDBusInterface : public QObject
{
    Q_OBJECT

public:
    CallAndSimDBusInterface();
    virtual ~CallAndSimDBusInterface();

public:
    void requestAllValues();

    void callerIdSendingRequired();
    void callWaitingRequired();
    void callForwardingRequired();
    void forwardToRequired();
    void pinRequestRequired();

public slots:
    void setCallerIdSending(int value);
    void setCallWaiting(bool enabled);
    void setCallForwarding(bool enabled);
    void setForwardTo(const QString& number);
    void setPinRequest(bool enabled);

private slots:
    void valueSet();
    void DBusMessagingFailure();

signals:
    void callerIdSending(int);
    void callWaiting(bool);
    void callForwarding(bool);
    void forwardTo(QString);
    void pinRequest(bool);

private:
    QDBusInterface* dbusIf;
};

#endif // CALLANDSIMDBUSINTERFACE_H
