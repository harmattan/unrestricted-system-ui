include(../common_top.pri)
TARGET = ut_lockscreenui

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

SOURCES += ut_lockscreenui.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/unlockwidgets.cpp \
    $$SRC_PREFIX/lockscreenui.cpp  \
    $$STUB_PREFIX/stubbase.cpp

HEADERS += ut_lockscreenui.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/unlockwidgets.h \
    $$SRC_PREFIX/lockscreenui.h \
    $$SRC_PREFIX/sysuid.h 

include(../common_bot.pri)

