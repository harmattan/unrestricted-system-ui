#ifndef LOCKSCREENBUSINESSLOGICSTUB_H
#define LOCKSCREENBUSINESSLOGICSTUB_H

#include <QObject>
#include <QTimer>

class LockScreenBusinessLogic : public QObject
{
    Q_OBJECT

public:
    LockScreenBusinessLogic (QObject* parent = 0);
    virtual ~LockScreenBusinessLogic ();

public slots:
    void toggleScreenLockUI (bool toggle);
    void toggleEventEater   (bool toggle);

private: //methods
    void mayStartTimer();
    void stopTimer();
    void hidefromTaskBar();
    void unlockScreen();

    // These are very useful for unit testing...
public: 
    bool            lockUIvisible;
    bool            eaterUIvisible;
};

#endif // LOCKSCREENBUSINESSLOGIC_H
