include(../common_top.pri)
TARGET = ft_lockdbusinterface

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

SOURCES += ft_lockdbusinterface.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$STUB_PREFIX/stubbase.cpp \
    $$SRC_PREFIX/debug.cpp 

HEADERS += ft_lockdbusinterface.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRC_PREFIX/sysuid.h \
    $$SRC_PREFIX/debug.h 

include(../common_bot.pri)

