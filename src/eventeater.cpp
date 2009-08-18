#include <QX11Info>
#include "eventeater.h"

EventEater::EventEater()
{
    QX11Info x11;

    int screen = x11.screen();
    XSetWindowAttributes attr;
    attr.event_mask =
            ButtonPressMask |
            ButtonReleaseMask |
            PointerMotionMask |
            KeyPressMask |
            KeyReleaseMask |
            ExposureMask |
            VisibilityChangeMask |
            PropertyChangeMask |
            FocusChangeMask;
    eventEater = XCreateWindow(x11.display(),
                                      DefaultRootWindow(x11.display()),
                                      0,0,
                                      DisplayWidth(x11.display(), screen),
                                      DisplayHeight(x11.display(), screen),
                                      0, // border
                                      0, // depth
                                      InputOnly,
                                      CopyFromParent,
                                      CWEventMask,
                                      &attr);
}

EventEater::~EventEater()
{
    QX11Info x11;
    XDestroyWindow(x11.display(), eventEater);
}

void EventEater::toggle(bool toggle)
{
    QX11Info x11;

    if(toggle)
        XMapRaised(x11.display(), eventEater);
    else
        XUnmapWindow(x11.display(), eventEater);
}
