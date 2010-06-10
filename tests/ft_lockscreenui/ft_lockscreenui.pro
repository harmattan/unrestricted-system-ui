include(../common_top.pri)
TARGET = ft_lockscreenui

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

PKGCONFIG += \
    qmsystem 

INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

SOURCES += ft_lockscreenui.cpp \
    xchecker.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/unlockwidgets.cpp \
    $$SRC_PREFIX/lockscreenui.cpp  \
    $$STUB_PREFIX/stubbase.cpp \
    $$SRC_PREFIX/unlockmissedevents.cpp \
    $$SRC_PREFIX/unlocknotifications.cpp

HEADERS += ft_lockscreenui.h \
    xchecker.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/unlockwidgets.h \
    $$SRC_PREFIX/lockscreenui.h \
    $$SRC_PREFIX/sysuid.h \
    $$SRC_PREFIX/unlockmissedevents.h \
    $$SRC_PREFIX/unlocknotifications.h

include(../common_bot.pri)

