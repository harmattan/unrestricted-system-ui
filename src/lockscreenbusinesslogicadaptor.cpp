#include "lockscreenbusinesslogicadaptor.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor(
        QObject *obj, LockScreenBusinessLogic *logic)
        : QDBusAbstractAdaptor(obj)
{
    connect(this, SIGNAL(delegateSetMissedEvents(int,int,int,int)),
            logic, SLOT(updateMissedEventAmounts(int,int,int,int)));
}

LockScreenBusinessLogicAdaptor::~LockScreenBusinessLogicAdaptor()
{
}

void LockScreenBusinessLogicAdaptor::SetMissedEvents(int a, int b, int c, int d)
{
    emit delegateSetMissedEvents(a, b, c, d);
}
