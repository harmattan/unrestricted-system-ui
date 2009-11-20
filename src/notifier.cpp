
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
    connect(notification, SIGNAL(buttonClicked()), this, SLOT(doKillTimer()));
    connect(notification, SIGNAL(clicked()), this, SLOT(doKillTimer()));
    timerId = startTimer(expireTimeout);
}

NotifTimer::~NotifTimer()
{
    if (timerId > 0)
        killTimer(timerId);
}

void NotifTimer::timerEvent(QTimerEvent *e)
{
    // need to kill the timer _before_ we emit timeout() in case the
    // slot connected to timeout calls processEvents()
    doKillTimer();
    emit timeout(notification);
    emit timeout(notifId);
    delete this;
}

void NotifTimer::doKillTimer()
{
    if (timerId > 0)
        killTimer(timerId);
    timerId = -1;
}

// TODO.: Use the logical names when available

// In the future the notifier connects to Notification Framework API and uses that to create notifications
// See messge formats from af/duihome:home/notifications/notificationmanager.xml;
// example message to test notificationManager:
//  dbus-send --print-reply --dest=org.maemo.dui.NotificationManager / org.maemo.dui.NotificationManager.addNotification uint32:0 string:'new-message' string:'Message received' string:'Hello DUI' string:'link' string:'Icon-close'
Notifier::Notifier(QObject* parent) :
        QObject(parent)
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

void Notifier::showNotification(const QString &notifText, NotificationType::Type type)
{
    switch (type)
    {
        case NotificationType::error:
            showDBusNotification(notifText, QString("error"));
            break;
        case NotificationType::warning:
            showDBusNotification(notifText, QString("warning"));
            break;
        case NotificationType::info:
            showDBusNotification(notifText, QString("info"));
            break;
    }
}

void Notifier::showConfirmation(const QString &notifText, const QString &buttonText)
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
        // because of bootup order, connection can't be done in constructor.
        bool res = connect(
                dbus, SIGNAL(showNotification(QString, NotificationType)),
                this, SLOT(showNotification(QString, NotificationType)));
        if(!res){
            showLocalNotification(expireTimeout, notifText, buttonText);
        }
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
    if(0 < expireTimeout){
        (void) new NotifTimer(expireTimeout, this, SLOT(notificationTimeout(unsigned int)), notifId);
    }
}

void Notifier::notifTimer(int expireTimeout, DuiInfoBanner* notif)
{
    qDebug() << Q_FUNC_INFO << "expireTimeout:" << expireTimeout;
    if(!notif){
        return;
    }

    if(0 < expireTimeout){
        (void) new NotifTimer(expireTimeout, this, SLOT(notificationTimeout(DuiInfoBanner*)), notif);
    }
    else if(0 >= notif->buttonText().length()){
        notif->disappear();
    }
}

void Notifier::showLocalNotification(int expireTimeout, QString notifText, QString buttonText)
{
    qDebug() << Q_FUNC_INFO << notifText;
    // DuiInfoBanner(BannerType type, const QString& image, const QString& body, const QString& iconId);
    DuiInfoBanner* n = new DuiInfoBanner(DuiInfoBanner::Information,
                                             "Icon-close",
                                             QString(notifText),
                                             "Icon-close");

    if (trid("qtn_cell_continue", "Continue") == buttonText){
        expireTimeout = 0;
        n->setButtonText(buttonText);
        connect(n, SIGNAL(buttonClicked()), this, SLOT(localNotificationPinQueryCancel()));
    }
    else if (trid("qtn_cell_try_again", "Try again") == buttonText){
        expireTimeout = 0;
        n->setButtonText(buttonText);
        connect(n, SIGNAL(buttonClicked()), this, SLOT(localNotificationSimLockRetry()));
    }
    connect(n, SIGNAL(clicked()), this, SLOT(localNotificationClose()));
    n->appear(DuiSceneWindow::DestroyWhenDone);
    notifTimer(expireTimeout, n);
}

void Notifier::localNotificationClose()
{
    DuiInfoBanner *ib = qobject_cast<DuiInfoBanner *>(sender());
    qDebug() << Q_FUNC_INFO << (QObject*) ib;
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


