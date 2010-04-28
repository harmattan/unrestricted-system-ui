include(../common_top.pri)
SRC_PREFIX = ../../src/
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX

QT += \
    testlib \
    dbus 

PKGCONFIG += \
    usb_moded \
    dbus-1

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_usbbusinesslogic
target.path = /usr/lib/system-ui-tests

CONFIG += \
	debug \
    gui \
    meegotouch \
	link_pkgconfig \
	silent 

HEADERS += \
    $$SRC_PREFIX/debug.h \
    ut_usbbusinesslogic.h \
    $$SRC_PREFIX/usbbusinesslogic.h \
    $$SRC_PREFIX/usbbusinesslogicadaptor.h

SOURCES += \
    $$SRC_PREFIX/debug.cpp \
    ut_usbbusinesslogic.cpp \
    $$SRC_PREFIX/usbbusinesslogic.cpp \
    $$SRC_PREFIX/usbbusinesslogicadaptor.cpp

include(../common_bot.pri)
