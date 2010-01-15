#ifndef LOCKSCREENBUSINESSLOGICADAPTOR_H
#define LOCKSCREENBUSINESSLOGICADAPTOR_H

#include "lockscreenbusinesslogic.h"

#include <QDBusAbstractAdaptor>


class LockScreenBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.LockScreen")

public:
    LockScreenBusinessLogicAdaptor(QObject *obj, LockScreenBusinessLogic *delegate);
    virtual ~LockScreenBusinessLogicAdaptor();

public slots:
    Q_NOREPLY void SetMissedEvents(int emails, int messages, int calls, int im);

signals:
    void delegateSetMissedEvents(int, int, int, int);

};

#endif // LOCKSCREENBUSINESSLOGICADAPTOR_H
