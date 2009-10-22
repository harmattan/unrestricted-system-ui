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
    void callerIdSendingRequired();

public slots:
    void setCallerIdSending(int value);

private slots:
    void valueSet();
    void DBusMessagingFailure();

signals:
    void callerIdSending(int);

private:
    QDBusInterface* dbusIf;
};

#endif // CALLANDSIMDBUSINTERFACE_H
