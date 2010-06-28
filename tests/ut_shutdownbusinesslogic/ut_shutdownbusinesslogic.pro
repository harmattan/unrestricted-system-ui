include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_shutdownbusinesslogic

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += \
    $$SRC_PREFIX \
    $$STUB_PREFIX

SOURCES += \
    ../ft_lockscreenui/xchecker.cpp \
    ut_shutdownbusinesslogic.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/shutdownbusinesslogic.cpp

HEADERS += \
    ../ft_lockscreenui/xchecker.h \
    ut_shutdownbusinesslogic.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/shutdownbusinesslogic.h

include(../common_bot.pri)

