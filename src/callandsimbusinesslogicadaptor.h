#ifndef CALLANDSIMBUSINESSLOGICADAPTOR_H
#define CALLANDSIMBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>

class CallAndSimBusinessLogic;

class CallAndSimBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.callandsim")

public:
    CallAndSimBusinessLogicAdaptor(QObject* obj, CallAndSimBusinessLogic* logic);
    virtual ~CallAndSimBusinessLogicAdaptor();

public slots:
    Q_NOREPLY void getCallerIdSending();
    Q_NOREPLY void getCallWaiting();
    Q_NOREPLY void getCallForwarding();
    Q_NOREPLY void getForwardTo();
    Q_NOREPLY void getPinRequest();

    Q_NOREPLY void setCallerIdSending(int value);
    Q_NOREPLY void setCallWaiting(bool enabled);
    Q_NOREPLY void setCallForwarding(bool enabled);
    Q_NOREPLY void setForwardTo(const QString& number);
    Q_NOREPLY void setPinRequest(bool enabled);

signals:
    void callerIdSending(int);
    void callWaiting(bool);
    void callForwarding(bool);
    void forwardTo(QString);
    void pinRequest(bool);

private:
    CallAndSimBusinessLogic* logic;
};

#endif // CALLANDSIMBUSINESSLOGICADAPTOR_H

