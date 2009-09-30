#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QHash>

class CancellableNotification;
class QDBusInterface;
class NotifierDBusAdaptor;
class Sysuid;

/* Class to keep track of each notification shown in homescreen */
class NotifTimer : public QObject
{
    Q_OBJECT
    int timerId;
public:
    ~NotifTimer();
    NotifTimer(int expireTimeout, QObject *receiver, const char *member, unsigned int notifId);
signals:
    void timeout(unsigned int notifId);
protected:
    void timerEvent(QTimerEvent *);
private:
    unsigned int notifId;
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
    void showCancellableNotification(QString notifText,
                                     int appearTime,
                                     QString appearTimeVariable,
                                     const QHash<QString,QString> &staticVariables);

private slots:
    void notificationTimeout(unsigned int notifId);
    void cancellableNotificationCancelled();
    void cancellableNotificationTimeout();

protected:
    Notifier();

private:
    /* expireTimeout tells when the note is removed (also from home screen). Value 5000 = 5 sec seems to be the time
     * the notification is removed from the visible view.
     */
    void showDBusNotification(QString notifText, QString evetType, QString summary = QString(), int expireTimeout = 5000, QString action = QString("removeNotification"));
    void removeNotification(unsigned int id);
    void notifTimer(int msec, unsigned int notifId);

private:    
    CancellableNotification *cancellableNotification;
    QDBusInterface* managerIf;
    NotifierDBusAdaptor* dbus;
};


#endif // NOTIFIER_H
