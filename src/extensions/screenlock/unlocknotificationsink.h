#ifndef UNLOCK_NOTIFICATIONS_H
#define UNLOCK_NOTIFICATIONS_H

#include <QObject>
#include "notificationsink.h"

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

class MGConfItem;

class UnlockNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    UnlockNotificationSink(QObject *parent = NULL);

protected slots:
    virtual void addNotification (const Notification &notification);
    virtual void removeNotification (uint notificationId);

#ifdef HAVE_QMSYSTEM
    void locksChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
#endif

protected:
    bool canAddNotification (const Notification &notification);

private:
    void setLockedState (bool islocked);

    bool m_enabled;

    //! GConf key for enabling/disabling private notifications
    MGConfItem *privateNotificationSetting;

#ifdef HAVE_QMSYSTEM
    MeeGo::QmLocks locks;
#endif

#ifdef UNIT_TEST
friend class Ut_UnlockNotificationSink;
#endif
};

#endif
