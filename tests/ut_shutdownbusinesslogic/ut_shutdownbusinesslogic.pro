include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_shutdownbusinesslogic

INCLUDEPATH += \
    $$SRCDIR \
    $$STUBSDIR

SOURCES += \
    ut_shutdownbusinesslogic.cpp \
    $$SRCDIR/shutdownbusinesslogic.cpp

HEADERS += \
    ut_shutdownbusinesslogic.h \
    $$SRCDIR/shutdownbusinesslogic.h

include(../common_bot.pri)

