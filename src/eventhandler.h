#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <qmsystem/qmkeys.h>
#include "lockscreenbusinesslogic.h"

using namespace Maemo;

class EventHandler : public QObject
{
    Q_OBJECT

public:
    EventHandler(LockScreenBusinessLogic *lockScreenLogic);
    virtual ~EventHandler();

private slots:
    void keyPressed(QmKeys::Keys key, QmKeys::HowPressed how);

private: //methods
    void shortPowerKeyPressOccured();

private: //atributes
    LockScreenBusinessLogic *lockScreenLogic;
    QmKeys *keys;

};

#endif // EVENTHANDLER_H
