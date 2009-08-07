#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QString>

class Notifier : QObject
{
    Q_OBJECT

public:
    Notifier();
    virtual ~Notifier();

public slots:    
    void showNotification(QString notifText);

private slots:
    void notificationTimeout();

};


#endif // NOTIFIER_H
