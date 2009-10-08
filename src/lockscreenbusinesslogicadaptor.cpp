#include "lockscreenbusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor(QObject *obj, LockScreenBusinessLogic *lockScreenLogic)
        : QDBusAbstractAdaptor(obj), lockScreenLogic(lockScreenLogic)
{
}

void LockScreenBusinessLogicAdaptor::addMissedCall()
{
}

void LockScreenBusinessLogicAdaptor::addUnreadMessage()
{
}

void LockScreenBusinessLogicAdaptor::addUnreadChatMessage()
{
}
