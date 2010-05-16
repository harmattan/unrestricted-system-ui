#ifndef UNLOCK_NOTIFICATIONS_H
#define UNLOCK_NOTIFICATIONS_H

#include <QObject>
#include "notifications/notificationsink.h"

class UnlockNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    UnlockNotificationSink ();
    void setLockedState (bool islocked);

signals:
    void updateNotificationsCount (int emails, int messages, int calls, int im);

protected slots:
    virtual void addNotification (const Notification &notification);
    virtual void removeNotification (uint notificationId);

protected:
    bool canAddNotification (const Notification &notification);

private:
    bool             m_enabled;
    int              m_emails;
    int              m_messages;
    int              m_calls;
    int              m_im;
};

#endif
