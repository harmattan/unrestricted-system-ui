#include "lockscreenbusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor(QObject *obj, LockScreenBusinessLogic *lockScreenLogic)
        : QDBusAbstractAdaptor(obj), lockScreenLogic(lockScreenLogic)
{    
}

void LockScreenBusinessLogicAdaptor::unreadMessagesAmountChanged(int amount)
{
    lockScreenLogic->unreadMessagesAmountChanged(amount);
}

void LockScreenBusinessLogicAdaptor::missedCallsAmountChanged(int amount)
{
    lockScreenLogic->missedCallsAmountChanged(amount);
}

void LockScreenBusinessLogicAdaptor::unreadChatMessagesAmountChanged(int amount)
{
    lockScreenLogic->unreadChatMessagesAmountChanged(amount);
}
