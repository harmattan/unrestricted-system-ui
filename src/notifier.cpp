
#include <DuiLocale>
#include <QTimer>
#include <QHash>
#include <QObject>
#include <QDebug>
#include <QDBusInterface>
#include <QAbstractEventDispatcher>
#include <DuiInfoBanner>

#include "notifier.h"
#include "sysuid.h"
#include "notifierdbusadaptor.h"


NotifTimer::NotifTimer(int expireTimeout, QObject *receiver, const char *member, unsigned int notifId) :
    QObject(QAbstractEventDispatcher::instance()),
    notifId(notifId),
    notification(NULL)
{
    connect(this, SIGNAL(timeout(unsigned int)), receiver, member);
    timerId = startTimer(expireTimeout);
}

NotifTimer::NotifTimer(int expireTimeout, QObject *receiver, const char *member, DuiInfoBanner* notification) :
    QObject(QAbstractEventDispatcher::instance()),
    notifId(-1),
    notification(notification)
{
    connect(this, SIGNAL(timeout(DuiInfoBanner*)), receiver, member);
    timerId = startTimer(expireTimeout);
}

NotifTimer::~NotifTimer()
{
    if (timerId > 0)
        killTimer(timerId);
}

void NotifTimer::timerEvent(QTimerEvent *)
{
    // need to kill the timer _before_ we emit timeout() in case the
    // slot connected to timeout calls processEvents()
    if (timerId > 0)
        killTimer(timerId);
    timerId = -1;
    emit timeout(notification);
    emit timeout(notifId);
    delete this;
}

// TODO.: Use the logical names when available

// In the future the notifier connects to Notification Framework API and uses that to create notifications
// See messge formats from af/duihome:home/notifications/notificationmanager.xml;
// example message to test notificationManager:
//  dbus-send --print-reply --dest=org.maemo.dui.NotificationManager / org.maemo.dui.NotificationManager.addNotification uint32:0 string:'new-message' string:'Message received' string:'Hello DUI' string:'link' string:'Icon-close'
Notifier::Notifier() : QObject()
{
    dbus = new NotifierDBusAdaptor();

    connect(dbus, SIGNAL(showNotification(QString,Notifier::NotificationType)), this, SLOT(showNotification(QString,Notifier::NotificationType)));

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

    qDebug() << Q_FUNC_INFO << "action:" << action;
    showDBusNotification(notifText, QString("confirmation"), QString(""), 0, action, buttonText);
}


void Notifier::notificationTimeout(unsigned int notifId)
{
    if(0 < notifId) {
        removeNotification(notifId);
    }

    QTimer *t = qobject_cast<QTimer *>(sender());
    qDebug() << Q_FUNC_INFO << "timer:" << t;
    if (t != NULL) {
        DuiInfoBanner *dn = qobject_cast<DuiInfoBanner *>(t->parent());
        qDebug() << Q_FUNC_INFO << "dn:" << (QObject*)dn;
        if (dn != NULL) {
            dn->disappear();
        }
    }
}

void Notifier::notificationTimeout(DuiInfoBanner* notif)
{
    qDebug() << Q_FUNC_INFO << "notif:" << (QObject*)notif;
    if (notif != NULL) {
        notif->disappear();
    }
}

void Notifier::removeNotification(unsigned int id)
{
    QDBusMessage reply = managerIf->call(
        QString("removeNotification"),
        QVariant((unsigned int)id));

    // practically just debugging...
    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "(" << id << ") error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        qDebug() << Q_FUNC_INFO << "(" << id << ") result:" << reply.arguments()[0].toUInt();
    }
    else {
        qDebug() << Q_FUNC_INFO << "(" << id << ") reply type:" << reply.type();
    }

}

void Notifier::showDBusNotification(QString notifText, QString evetType, QString summary, int expireTimeout, QString action, QString buttonText)
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
        qDebug() << Q_FUNC_INFO << " error reply:" << reply.errorName();
        showLocalNotification(expireTimeout, notifText, buttonText);
    }
    else if(reply.type() == QDBusMessage::ReplyMessage)
    {
        unsigned int notifId(0);
        QList<QVariant> args = reply.arguments();
        if(args.count() >= 1)
        {
            notifId = args[0].toUInt();
            qDebug() << Q_FUNC_INFO << ": notifId:" << notifId << "msg:" << notifText;
        }
        notifTimer(expireTimeout, notifId);
    }
    else {
        qDebug() << Q_FUNC_INFO << "reply type:" << reply.type();
    }
}

void Notifier::notifTimer(int expireTimeout, unsigned int notifId)
{
    if(0 < expireTimeout)
        (void) new NotifTimer(expireTimeout, this, SLOT(notificationTimeout(unsigned int)), notifId);
}

void Notifier::notifTimer(int expireTimeout, DuiInfoBanner* notif)
{
    qDebug() << Q_FUNC_INFO << "expireTimeout:" << expireTimeout;
    if(0 < expireTimeout)
        (void) new NotifTimer(expireTimeout, this, SLOT(notificationTimeout(DuiInfoBanner*)), notif);
    else if(0 >= notif->buttonText().length())
        notif->disappear();
}

void Notifier::showLocalNotification(int expireTimeout, QString notifText, QString buttonText)
{
    qDebug() << Q_FUNC_INFO << notifText;
    // DuiInfoBanner(BannerType type, const QString& image, const QString& body, const QString& iconId);
    DuiInfoBanner* n = new DuiInfoBanner(DuiInfoBanner::Information,
                                             "Icon-close",
                                             QString(notifText),
                                             "Icon-close");
/*    DuiInfoBanner* n = new DuiInfoBanner(DuiInfoBanner::Information,
                                             "Icon-close",
                                             QString( "<font color=\"white\">" + notifText + "</font>"),
                                             "Icon-close");
*/
    if (trid("qtn_cell_continue", "Continue") == buttonText){
        expireTimeout = 0;
        n->setButtonText(buttonText);
        connect(n, SIGNAL(buttonClicked()), this, SLOT(localNotificationPinQueryCancel()));
        connect(n, SIGNAL(clicked()), this, SLOT(localNotificationPinQueryCancel()));
    }
    else if (trid("qtn_cell_try_again", "Try again") == buttonText){
        expireTimeout = 0;
        n->setButtonText(buttonText);
        connect(n, SIGNAL(buttonClicked()), this, SLOT(localNotificationSimLockRetry()));
        connect(n, SIGNAL(clicked()), this, SLOT(localNotificationSimLockRetry()));
    } else {
        connect(n, SIGNAL(clicked()), this, SLOT(localNotificationClose()));
    }
    n->appear(DuiSceneWindow::DestroyWhenDone);
    notifTimer(expireTimeout, n);
}

void Notifier::localNotificationClose()
{
    DuiInfoBanner *ib = qobject_cast<DuiInfoBanner *>(sender());
    if (ib != NULL) {
        ib->disappear();
    }
}
void Notifier::localNotificationPinQueryCancel()
{
    localNotificationClose();
    dbus->pinQueryCancel();
}

void Notifier::localNotificationSimLockRetry()
{
    localNotificationClose();
    dbus->simLockRetry();
}


