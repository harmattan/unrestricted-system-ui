#ifndef UNLOCKNOTIFICATIONSINKSTUB_H
#define UNLOCKNOTIFICATIONSINKSTUB_H

#include <unlocknotificationsink.h>

UnlockNotificationSink::UnlockNotificationSink (QObject *parent) : NotificationSink(parent)
{
    m_enabled = true;
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

void
UnlockNotificationSink::setLockedState(bool islocked)
{
    Q_UNUSED (islocked);
}

#ifdef HAVE_QMSYSTEM
void UnlockNotificationSink::locksChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)
{
}
#endif

#endif
