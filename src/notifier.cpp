#include "notifier.h"
#include "cancellablenotification.h"

#include <DuiNotification>
#include <DuiLocale>
#include <QTimer>
#include <QHash>
#include <QObject>


// TODO.: Use the logical names when available

// In the future the notifier connects to Notification Framework API and uses that to create notifications

Notifier::Notifier() : QObject()
{
}

Notifier::~Notifier()
{
    delete cancellableNotification;
    cancellableNotification = NULL;
}

void Notifier::showNotification(QString notifText)
{
    DuiNotification* n = new DuiNotification(DuiNotification::Information,
                                             "Icon-close",
                                             QString( "<font color=\"white\">" + notifText + "</font>"),
                                             "");
    QTimer *t = new QTimer(n);
    connect(t, SIGNAL(timeout()), this, SLOT(notificationTimeout()));
    t->setSingleShot(true);
    t->start(3000);
    n->appear(DuiSceneWindow::DestroyWhenDone);
}

void Notifier::notificationTimeout()
{
    QTimer *t = qobject_cast<QTimer *>(sender());

    if (t != NULL) {
        DuiNotification *dn = qobject_cast<DuiNotification *>(t->parent());
        if (dn != NULL) {
            dn->disappear();
        }
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
