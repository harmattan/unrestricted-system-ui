include(../common_top.pri)
TARGET = ft_batterybusinesslogic

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

HEADERS += \
    ft_batterybusinesslogic.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/batterybusinesslogic.h \
    $$STUB_PREFIX/stubbase.h \
    $$STUB_PREFIX/qmdevicemode_stub.h \
    $$STUB_PREFIX/qmbattery_stub.h \
    $$STUB_PREFIX/qmled_stub.h

SOURCES += \
    ft_batterybusinesslogic.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/batterybusinesslogic.cpp \
    $$STUB_PREFIX/stubbase.cpp

include(../common_bot.pri)
