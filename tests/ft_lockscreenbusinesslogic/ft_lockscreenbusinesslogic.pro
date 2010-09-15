include(../common_top.pri)
TARGET = ft_lockscreenbusinesslogic

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

SOURCES += ft_lockscreenbusinesslogic.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/unlockmissedevents.cpp \
    $$SRCDIR/lockscreenbusinesslogic.cpp \
    $$SRCDIR/unlockwidgets.cpp \
    $$SRCDIR/lockscreenwindow.cpp \
    $$SRCDIR/lockscreen.cpp \
    $$SRCDIR/eventeater.cpp  \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/unlocknotifications.cpp

HEADERS += ft_lockscreenbusinesslogic.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/unlockmissedevents.h \
    $$SRCDIR/lockscreenbusinesslogic.h \
    $$SRCDIR/unlockwidgets.h \
    $$SRCDIR/lockscreenwindow.h \
    $$SRCDIR/lockscreen.h \
    $$SRCDIR/eventeater.h  \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/unlocknotifications.h

include(../common_bot.pri)

