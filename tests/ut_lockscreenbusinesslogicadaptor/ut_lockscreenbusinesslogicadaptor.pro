include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenbusinesslogicadaptor
INCLUDEPATH += $$SRCDIR/screenlock

# unit test and unit
SOURCES += \
    $$SRCDIR/screenlock/screenlockbusinesslogicadaptor.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_lockscreenbusinesslogicadaptor.cpp

HEADERS += \
    $$SRCDIR/screenlock/screenlockbusinesslogicadaptor.h \
    $$SRCDIR/screenlock/screenlockbusinesslogic.h \
    ut_lockscreenbusinesslogicadaptor.h

include(../common_bot.pri)
