include(../common_top.pri)
TARGET = ft_lockscreenbusinesslogic

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

SOURCES += ft_lockscreenbusinesslogic.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/lockscreenbusinesslogic.cpp \
    $$SRC_PREFIX/unlockwidgets.cpp \
    $$SRC_PREFIX/lockscreenui.cpp  \
    $$STUB_PREFIX/stubbase.cpp \
    $$SRC_PREFIX/unlocknotifications.cpp

HEADERS += ft_lockscreenbusinesslogic.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/lockscreenbusinesslogic.h \
    $$SRC_PREFIX/unlockwidgets.h \
    $$SRC_PREFIX/lockscreenui.h \
    $$SRC_PREFIX/sysuid.h \
    $$SRC_PREFIX/unlocknotifications.h

include(../common_bot.pri)

