#ifndef EventEater_H
#define EventEater_H

#include <qmsystem/qmkeys.h>
#include "lockscreenbusinesslogic.h"

using namespace Maemo;

class EventEater : public QObject
{
    Q_OBJECT

public:
    EventEater(LockScreenBusinessLogic *lockScreenLogic);
    virtual ~EventEater();

private slots:
    void keyPressed(QmKeys::Keys key, QmKeys::HowPressed how);

private: //methods
    void shortPowerKeyPressOccured();

private: //atributes
    LockScreenBusinessLogic *lockScreenLogic;
    QmKeys *keys;

};

#endif // LOCKSCREENUI_H
