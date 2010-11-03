include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_shutdownui

INCLUDEPATH += \
    $$SRCDIR \
    $$STUBSDIR

SOURCES += \
    ut_shutdownui.cpp \
    $$SRCDIR/shutdownui.cpp

HEADERS += \
    ut_shutdownui.h \
    $$SRCDIR/shutdownui.h

include(../common_bot.pri)

