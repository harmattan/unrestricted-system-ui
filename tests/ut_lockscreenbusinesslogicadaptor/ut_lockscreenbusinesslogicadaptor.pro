include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenbusinesslogicadaptor
INCLUDEPATH += $$SRCDIR/lockscreen

# unit test and unit
SOURCES += \
    $$SRCDIR/lockscreen/lockscreenbusinesslogicadaptor.cpp \
    $$STUBSDIR/lockscreenbusinesslogic_stub.cpp \
    ut_lockscreenbusinesslogicadaptor.cpp

HEADERS += \
    $$SRCDIR/lockscreen/lockscreenbusinesslogicadaptor.h \
    $$STUBSDIR/lockscreenbusinesslogic_stub.h \
    ut_lockscreenbusinesslogicadaptor.h

include(../common_bot.pri)
