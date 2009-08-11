#ifndef EventEater_H
#define EventEater_H

#include <DuiApplicationPage>

class EventEater : public DuiApplicationPage
{
    Q_OBJECT

public:
    EventEater();
    virtual ~EventEater();

private:
	int tummy;
};

#endif // LOCKSCREENUI_H
