include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_batterybusinesslogic

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

HEADERS += \
    ut_batterybusinesslogic.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/batterybusinesslogic.h \
    $$STUB_PREFIX/qmled_stub.h \
    $$STUB_PREFIX/qmbattery_stub.h \
    $$STUB_PREFIX/qmdevicemode_stub.h \
    $$STUB_PREFIX/stubbase.h

SOURCES += \
    ut_batterybusinesslogic.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/batterybusinesslogic.cpp \
    $$STUB_PREFIX/stubbase.cpp

include(../common_bot.pri)
