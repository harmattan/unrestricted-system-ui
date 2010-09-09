include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_shutdownui

 
 

INCLUDEPATH += \
    $$SRCDIR \
    $$STUBSDIR

SOURCES += \
    ../ft_lockscreenui/xchecker.cpp \
    ut_shutdownui.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/shutdownui.cpp

HEADERS += \
    ../ft_lockscreenui/xchecker.h \
    ut_shutdownui.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/shutdownui.h

include(../common_bot.pri)

