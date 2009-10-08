#ifndef LOCKSCREENBUSINESSLOGICADAPTOR_H
#define LOCKSCREENBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>

class LockScreenBusinessLogic;

class LockScreenBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.lockscreen")

public:
    LockScreenBusinessLogicAdaptor(QObject *obj, LockScreenBusinessLogic *lockScreenLogic);

public slots:
    Q_NOREPLY void addMissedCall();
    Q_NOREPLY void addUnreadMessage();
    Q_NOREPLY void addUnreadChatMessage();

private:
    LockScreenBusinessLogic *lockScreenLogic;

};

#endif // LOCKSCREENBUSINESSLOGICADAPTOR_H
