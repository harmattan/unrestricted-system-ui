#ifndef UNLOCK_NOTIFICATIONS_H
#define UNLOCK_NOTIFICATIONS_H

#include <QObject>
#include "notifications/notificationsink.h"

class UnlockNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    UnlockNotificationSink ();

public slots:
    void setLockedState (bool islocked);

protected slots:
    virtual void addNotification (const Notification &notification);
    virtual void removeNotification (uint notificationId);

protected:
    bool canAddNotification (const Notification &notification);

private:
    bool             m_enabled;

#ifdef UNIT_TEST
friend class Ut_UnlockNotificationSink;
#endif
};

#endif
