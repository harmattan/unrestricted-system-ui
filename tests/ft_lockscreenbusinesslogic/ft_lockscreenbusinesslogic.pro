include(../common_top.pri)
TARGET = ft_lockscreenbusinesslogic
PKGCONFIG += xcomposite

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen

STYLE_HEADERS += $$SRCDIR/lockscreen/lockscreenwindowstyle.h

SOURCES += ft_lockscreenbusinesslogic.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/lockscreen/unlockmissedevents.cpp \
    $$SRCDIR/lockscreen/lockscreenbusinesslogic.cpp \
    $$SRCDIR/lockscreen/unlockarea.cpp \
    $$SRCDIR/lockscreen/lockscreenwindow.cpp \
    $$SRCDIR/lockscreen/lockscreen.cpp \
    $$SRCDIR/lockscreen/eventeater.cpp  \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/lockscreen/unlocknotifications.cpp \
    $$SRCDIR/x11wrapper.cpp

HEADERS += ft_lockscreenbusinesslogic.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/lockscreen/unlockmissedevents.h \
    $$SRCDIR/lockscreen/lockscreenbusinesslogic.h \
    $$SRCDIR/lockscreen/unlockarea.h \
    $$SRCDIR/lockscreen/lockscreenwindow.h \
    $$SRCDIR/lockscreen/lockscreenwindowstyle.h \
    $$SRCDIR/lockscreen/lockscreen.h \
    $$SRCDIR/lockscreen/eventeater.h  \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/lockscreen/unlocknotifications.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

