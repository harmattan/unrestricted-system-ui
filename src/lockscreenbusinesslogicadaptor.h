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
    Q_NOREPLY void unreadMessagesAmountChanged(int amount);
    Q_NOREPLY void missedCallsAmountChanged(int amount);
    Q_NOREPLY void unreadChatMessagesAmountChanged(int amount);

private:
    LockScreenBusinessLogic *lockScreenLogic;

};

#endif // LOCKSCREENBUSINESSLOGICADAPTOR_H
