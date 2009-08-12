#ifndef LOCKSCREENBUSINESSLOGIC_H
#define LOCKSCREENBUSINESSLOGIC_H

#include <QObject>

class LockScreenBusinessLogic : public QObject
{
    Q_OBJECT
public:
    LockScreenBusinessLogic();
    virtual ~LockScreenBusinessLogic();

signals:
    void lockScreenOff();

private slots:
/*
    -incoming events
        -call
        -alarm
    -power key press
    -timers
    -what else..?
*/
};

#endif // LOCKSCREENBUSINESSLOGIC_H
