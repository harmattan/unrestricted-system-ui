include(../common_top.pri)
TARGET = ut_lowbatterynotifier

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

CONFIG += silent \
          link_pkgconfig

PKGCONFIG += qmsystem

QT += gui

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/batterybusinesslogic.cpp \
    $$STUB_PREFIX/displaystatestub.cpp \
    $$STUB_PREFIX/batterystub.cpp \
    $$STUB_PREFIX/gconfstub.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/batterybusinesslogic.h \
    $$STUB_PREFIX/displaystatestub.h \
    $$STUB_PREFIX/batterystub.h \
    $$STUB_PREFIX/gconfstub.h

include(../common_bot.pri)
