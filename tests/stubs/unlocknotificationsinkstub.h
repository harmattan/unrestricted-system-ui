#ifndef UNLOCKNOTIFICATIONSINKSTUB_H
#define UNLOCKNOTIFICATIONSINKSTUB_H

#include <unlocknotificationsink.h>

UnlockNotificationSink::UnlockNotificationSink ()
{

}

void
UnlockNotificationSink::addNotification(const Notification &notification)
{
    Q_UNUSED (notification);
}

void
UnlockNotificationSink::removeNotification(uint notificationId)
{
    Q_UNUSED (notificationId);
}

#endif
