include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_shutdownui

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += \
    $$SRC_PREFIX \
    $$STUB_PREFIX

SOURCES += \
    ../ft_lockscreenui/xchecker.cpp \
    ut_shutdownui.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/shutdownui.cpp

HEADERS += \
    ../ft_lockscreenui/xchecker.h \
    ut_shutdownui.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/shutdownui.h

include(../common_bot.pri)

