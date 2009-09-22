#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QHash>

class CancellableNotification;
class QDBusInterface;

class Notifier : public QObject
{
    Q_OBJECT

public:
    // temporary event-types for notifications, defined as "what I want".
    // correct types aren't specified yet by duihome.
    // type is always mapped to 'new-message' --> shows envelope icon in info banner.
    enum NotificationType {
        error,
        info,
        warning
    };

    Notifier();
    virtual ~Notifier();

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
    void notificationTimeout();
    void cancellableNotificationCancelled();
    void cancellableNotificationTimeout();

private:
    void showDBusNotification(QString notifText, QString evetType, QString summary = QString(), int expireTimeout = 3000, QString action = QString("removeNotification"));
    void removeNotification(unsigned int id);

private:    
    CancellableNotification *cancellableNotification;
    QDBusInterface* managerIf;
    unsigned int notifId;

};


#endif // NOTIFIER_H
