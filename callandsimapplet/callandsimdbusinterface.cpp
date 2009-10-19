#include "callandsimdbusinterface.h"

#include <QDBusInterface>
#include <QDebug>

CallAndSimDBusInterface::CallAndSimDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/",
                                "com.nokia.systemui.callandsim",
                                QDBusConnection::sessionBus());

    connect(dbusIf, SIGNAL(callerIdSending(int)), this, SIGNAL(callerIdSending(int)));
    connect(dbusIf, SIGNAL(callWaiting(bool)),    this, SIGNAL(callWaiting(bool)));
    connect(dbusIf, SIGNAL(callForwarding(bool)), this, SIGNAL(callForwarding(bool)));
    connect(dbusIf, SIGNAL(forwardTo(QString)),   this, SIGNAL(forwardTo(QString)));
    connect(dbusIf, SIGNAL(pinRequest(bool)),     this, SIGNAL(pinRequest(bool)));
}

CallAndSimDBusInterface::~CallAndSimDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void CallAndSimDBusInterface::requestAllValues()
{
    qDebug() << Q_FUNC_INFO;

    callerIdSendingRequired();
    callWaitingRequired();
    callForwardingRequired();
    forwardToRequired();
    pinRequestRequired();
}

void CallAndSimDBusInterface::callerIdSendingRequired()
{
    qDebug() << Q_FUNC_INFO;
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getCallerIdSending"), list, this, SIGNAL(callerIdSending(int)), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::callWaitingRequired()
{
    qDebug() << Q_FUNC_INFO;
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getCallWaiting"), list, this, SIGNAL(callWaiting(bool)), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::callForwardingRequired()
{
    qDebug() << Q_FUNC_INFO;
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getCallForwarding"), list, this, SIGNAL(callForwarding(bool)), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::forwardToRequired()
{
    qDebug() << Q_FUNC_INFO;
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getForwardTo"), list, this, SIGNAL(forwardTo(QString)), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::pinRequestRequired()
{
    qDebug() << Q_FUNC_INFO;
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getPinRequest"), list, this, SIGNAL(pinRequest(bool)), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::setCallerIdSending(int value)
{
    qDebug() << Q_FUNC_INFO << (int)value;
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setCallerIdSending"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::setCallWaiting(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;
    QList<QVariant> list;
    list << QVariant(enabled);
    dbusIf->callWithCallback(QString("setCallWaiting"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::setCallForwarding(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;
    QList<QVariant> list;
    list << QVariant(enabled);
    dbusIf->callWithCallback(QString("setCallForwarding"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::setForwardTo(const QString& number)
{
    qDebug() << Q_FUNC_INFO << number;
    QList<QVariant> list;
    list << QVariant(number);
    dbusIf->callWithCallback(QString("setForwardTo"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::setPinRequest(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;
    QList<QVariant> list;
    list << QVariant(enabled);
    dbusIf->callWithCallback(QString("setPinRequest"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::valueSet()
{
    qDebug() << Q_FUNC_INFO;
}

void CallAndSimDBusInterface::DBusMessagingFailure()
{
    qDebug() << Q_FUNC_INFO;
}
