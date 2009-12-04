#include "callandsimdbusinterface.h"

#include <QDBusInterface>
#include <QDebug>

CallAndSimDBusInterface::CallAndSimDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/",
                                "com.nokia.systemui.callandsim",
                                QDBusConnection::sessionBus(), this);

    connect(dbusIf, SIGNAL(callerIdSending(int)), this, SIGNAL(callerIdSending(int)));
}

CallAndSimDBusInterface::~CallAndSimDBusInterface()
{
//    delete dbusIf;
//    dbusIf = NULL;
}

void CallAndSimDBusInterface::callerIdSendingRequired()
{
    qDebug() << Q_FUNC_INFO;
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getCallerIdSending"), list, this, SIGNAL(callerIdSending(int)), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::setCallerIdSending(int value)
{
    qDebug() << Q_FUNC_INFO << (int)value;
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setCallerIdSending"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void CallAndSimDBusInterface::valueSet()
{
    qDebug() << Q_FUNC_INFO;
}

void CallAndSimDBusInterface::DBusMessagingFailure()
{
    qDebug() << Q_FUNC_INFO;
}
