include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenbusinesslogic
INCLUDEPATH += $$SRCDIR/lockscreen

SOURCES += ut_lockscreenbusinesslogic.cpp \
    $$SRCDIR/lockscreen/lockscreenbusinesslogic.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreenbusinesslogic.h \
    $$SRCDIR/lockscreen/lockscreenbusinesslogic.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/lockscreen/lockscreen.h \
    $$SRCDIR/lockscreen/lockscreenwindow.h \
    $$SRCDIR/lockscreen/eventeater.h \

include(../common_bot.pri)

