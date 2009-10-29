#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QHash>

class CancellableNotification;
class QDBusInterface;
class NotifierDBusAdaptor;
class Sysuid;
class DuiInfoBanner;

/* Class to keep track of each notification shown in homescreen */
class NotifTimer : public QObject
{
    Q_OBJECT
    int timerId;
public:
    ~NotifTimer();
    NotifTimer(int expireTimeout, QObject *receiver, const char *member, unsigned int notifId);
    NotifTimer(int expireTimeout, QObject *receiver, const char *member, DuiInfoBanner* notification);
signals:
    void timeout(unsigned int notifId);
    void timeout(DuiInfoBanner* notification);
protected:
    void timerEvent(QTimerEvent *);
private:
    unsigned int notifId;
    DuiInfoBanner* notification;
};


/**
  * Class to create notifications
  */
class Notifier : public QObject
{
    Q_OBJECT

   friend class Sysuid;

public:
    // temporary event-types for notifications, defined as "what I want".
    // correct types aren't specified yet by duihome.
    // type is always mapped to 'new-message' --> shows envelope icon in info banner.
    enum NotificationType {
        error,
        info,
        warning
    };

    virtual ~Notifier();
    QObject* responseObject();

signals:
    void notifTimeout();

public slots:    
    void showNotification(QString notifText, Notifier::NotificationType type = info);
    void showConfirmation(QString notifText, QString buttonText);

private slots:
    void notificationTimeout(unsigned int notifId);
    void notificationTimeout(DuiInfoBanner* notification);
    void localNotificationClose();
    void localNotificationPinQueryCancel();
    void localNotificationSimLockRetry();

protected:
    Notifier(QObject* parent = 0);

private:
    /* expireTimeout tells when the note is removed (also from home screen).
     * Refer to duihome: NotificationManager::NotificationManager(int relayInterval = 3000, uint maxWaitQueueSize = 100);.
     */
    void showDBusNotification(QString notifText, QString evetType, QString summary = QString(), int expireTimeout = 3000, QString action = QString("removeNotification"), QString buttonText = QString(""));
    void removeNotification(unsigned int id);
    void notifTimer(int msec, unsigned int notifId);
    void notifTimer(int msec, DuiInfoBanner* notif);
    void showLocalNotification(int expireTimeout, QString notifText, QString buttonText);

private:    
    CancellableNotification *cancellableNotification;
    QDBusInterface* managerIf;
    NotifierDBusAdaptor* dbus;
};


#endif // NOTIFIER_H
