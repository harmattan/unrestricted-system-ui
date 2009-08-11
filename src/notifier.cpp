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
    DuiNotification* n = new DuiNotification(DuiNotification::Information,
                                             "Icon-close",
                                             QString( "<font color=\"white\">temptext</font>"),
                                             "");

    connect(n, SIGNAL(clicked()), this, SLOT(cancellableNotificationCancelled()));

    cancellableNotification = new CancellableNotification(n, notifText, appearTime, appearTimeVariable, staticVariables);
    connect(cancellableNotification, SIGNAL(notifTimeout()), this, SLOT(cancellableNotificationTimeout()));
    n->appear(DuiSceneWindow::DestroyWhenDone);
}

void Notifier::cancellableNotificationTimeout()
{
    delete cancellableNotification;
    cancellableNotification = NULL;
    emit notifTimeout();
}

void Notifier::cancellableNotificationCancelled()
{    
    delete cancellableNotification;
    cancellableNotification = NULL;
    DuiNotification *dn = qobject_cast<DuiNotification *>(sender());
    dn->disappear();
}
