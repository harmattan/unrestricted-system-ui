#ifndef EVENTEATER_H
#define EVENTEATER_H

#include <X11/Xlib.h>

class EventEater
{
public:
    EventEater();
    virtual ~EventEater();

    void toggle(bool toggle);

private:
    Window eventEater;

};

#endif // EVENTEATER_H

