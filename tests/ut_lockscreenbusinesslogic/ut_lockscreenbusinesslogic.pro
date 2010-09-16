include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenbusinesslogic

SOURCES += ut_lockscreenbusinesslogic.cpp \
    $$SRCDIR/lockscreenbusinesslogic.cpp \
    $$SRCDIR/debug.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreenbusinesslogic.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/lockscreenbusinesslogic.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/lockscreen.h \
    $$SRCDIR/lockscreenwindow.h \
    $$SRCDIR/eventeater.h \

include(../common_bot.pri)

