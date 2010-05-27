include(../common_top.pri)
TARGET = ft_shutdownui

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

SOURCES += ft_shutdownui.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$STUB_PREFIX/stubbase.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/shutdownui.cpp 

HEADERS += ft_shutdownui.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/sysuid.h \
    $$SRC_PREFIX/shutdownui.h 

include(../common_bot.pri)

