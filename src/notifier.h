#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QHash>

class CancellableNotification;

class Notifier : public QObject
{
    Q_OBJECT

public:
    Notifier();
    virtual ~Notifier();

signals:
    void notifTimeout();

public slots:    
    void showNotification(QString notifText);
    void showCancellableNotification(QString notifText,
                                     int appearTime,
                                     QString appearTimeVariable,
                                     const QHash<QString,QString> &staticVariables);

private slots:
    void notificationTimeout();
    void cancellableNotificationCancelled();
    void cancellableNotificationTimeout();

private:    
    CancellableNotification *cancellableNotification;

};


#endif // NOTIFIER_H
