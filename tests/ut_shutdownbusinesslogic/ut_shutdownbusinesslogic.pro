include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_shutdownbusinesslogic

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += \
    $$SRC_PREFIX \
    $$STUB_PREFIX

SOURCES += \
    ut_shutdownbusinesslogic.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/shutdownbusinesslogic.cpp

HEADERS += \
    ut_shutdownbusinesslogic.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/shutdownbusinesslogic.h

include(../common_bot.pri)

