
#include <DuiLocale>
#include <QTimer>
#include <QHash>
#include <QObject>
#include <QDebug>
#include <QDBusInterface>

#include "notifier.h"
#include "sysuid.h"
#include "cancellablenotification.h"
#include "notifierdbusadaptor.h"

// TODO.: Use the logical names when available

// In the future the notifier connects to Notification Framework API and uses that to create notifications
// See messge formats from af/duihome:home/notifications/notificationmanager.xml;
// example message to test notificationManager:
//  dbus-send --print-reply --dest=org.maemo.dui.NotificationManager / org.maemo.dui.NotificationManager.addNotification uint32:0 string:'new-message' string:'Message received' string:'Hello DUI' string:'link' string:'Icon-close'


Notifier::Notifier() : QObject(), notifId(0)
{
    dbus = new NotifierDBusAdaptor();
    managerIf = new QDBusInterface ( "org.maemo.dui.NotificationManager", "/", "org.maemo.dui.NotificationManager");
}

Notifier::~Notifier()
{    
    if(NULL != dbus)
        delete dbus;

    dbus = NULL;
    delete managerIf;
    managerIf = NULL;
}

QObject* Notifier::responseObject()
{
    return (QObject*)dbus;
}

void Notifier::showNotification(QString notifText, Notifier::NotificationType type)
{
    switch (type)
    {
        case error:
            showDBusNotification(notifText, QString("error"));
            break;
        case warning:
            showDBusNotification(notifText, QString("warning"));
            break;
        default:
            showDBusNotification(notifText, QString("info"));
            break;
    }
}

void Notifier::showConfirmation(QString notifText, QString buttonText)
{
/*
	// from DuiRemoteAction:
     
	QStringList l = string.split(' ');
    if (l.count() > 3) {
        d->serviceName = l.at(0);
        d->objectPath = l.at(1);
        d->interface = l.at(2);
        d->methodName = l.at(3);
    }
*/
    QString action(Sysuid::dbusService() + " " + Sysuid::dbusPath() + " " + NotifierDBusAdaptor::dbusInterfaceName() + " ");

    if (trid("qtn_cell_continue", "Continue") == buttonText)
        action += "pinQueryCancel";
    else if (trid("qtn_cell_try_again", "Try again") == buttonText)
        action += "simLockRetry";

    qDebug() << "Notifier::showConfirmation() action:" << action;
    showDBusNotification(notifText, QString("confirmation"), QString(""), 0, action);
}


void Notifier::notificationTimeout()
{
    if(0 < notifId) {
        removeNotification(notifId);
        notifId = 0;
    }
}

void Notifier::removeNotification(unsigned int id)
{
    QDBusMessage reply = managerIf->call(
        QString("removeNotification"),
        QVariant((unsigned int)id));

    // practically just debugging...
    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Notifier::removeNotification(" << id << ") error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        qDebug() << "Notifier::removeNotification(" << id << ") result:" << reply.arguments()[0].toUInt();
    }
    else {
        qDebug() << "Notifier::showDBusNotification(" << id << ") reply type:" << reply.type();
    }

}

void Notifier::showDBusNotification(QString notifText, QString evetType, QString summary, int expireTimeout, QString action)
{
    QDBusMessage reply = managerIf->call(
               QString("addNotification"),
               QVariant((unsigned int)0),
               QVariant(evetType), // temporary; correct types not specified yet; always shows envelope in info banner.
               QVariant(summary),
               QVariant(notifText),
               QVariant(action),
               QVariant(QString("Icon-close")));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Notifier::showDBusNotification() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage)
    {
        QList<QVariant> args = reply.arguments();
        if(args.count() >= 1)
        {
            notifId = args[0].toUInt();
            qDebug() << "Notifier::showDBusNotification(): notifId:" << notifId << "msg:" << notifText;
        }
        if(expireTimeout > 0)
        {
            QTimer::singleShot(expireTimeout, this, SLOT(notificationTimeout()));
        }
    }
    else {
        qDebug() << "Notifier::showDBusNotification() reply type:" << reply.type();
    }
}

void Notifier::showCancellableNotification(QString notifText,
                                           int appearTime,
                                           QString appearTimeVariable,
                                           const QHash<QString,QString> &staticVariables)
{
    cancellableNotification = new CancellableNotification(notifText, appearTime, appearTimeVariable, staticVariables);
    connect(cancellableNotification, SIGNAL(cancelled()), this, SLOT(cancellableNotificationCancelled()));
    connect(cancellableNotification, SIGNAL(timeout()), this, SLOT(cancellableNotificationTimeout()));
    cancellableNotification->show();
}

void Notifier::cancellableNotificationCancelled()
{        
    qDebug() << "cancelled";
    delete cancellableNotification;
    cancellableNotification = NULL;
}

void Notifier::cancellableNotificationTimeout()
{
    delete cancellableNotification;
    cancellableNotification = NULL;
    emit notifTimeout();
}
