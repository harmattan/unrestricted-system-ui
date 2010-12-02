#ifndef UNLOCK_NOTIFICATIONS_H
#define UNLOCK_NOTIFICATIONS_H

#include <QObject>
#include "notificationsink.h"

class MGConfItem;

class UnlockNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    UnlockNotificationSink(QObject *parent = NULL);

public slots:
    void setLockedState (bool islocked);

protected slots:
    virtual void addNotification (const Notification &notification);
    virtual void removeNotification (uint notificationId);

protected:
    bool canAddNotification (const Notification &notification);

private:
    bool m_enabled;

    //! GConf key for enabling/disabling private notifications
    MGConfItem *privateNotificationSetting;

#ifdef UNIT_TEST
friend class Ut_UnlockNotificationSink;
#endif
};

#endif
