include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenbusinesslogic
INCLUDEPATH += $$SRCDIR/screenlock

SOURCES += ut_lockscreenbusinesslogic.cpp \
    $$SRCDIR/screenlock/screenlockbusinesslogic.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreenbusinesslogic.h \
    $$SRCDIR/screenlock/screenlockbusinesslogic.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/screenlock/screenlockwindow.h \
    $$SRCDIR/screenlock/eventeater.h \
    $$SRCDIR/closeeventeater.h

include(../common_bot.pri)

