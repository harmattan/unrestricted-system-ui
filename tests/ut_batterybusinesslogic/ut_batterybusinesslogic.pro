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
    $$STUB_PREFIX/devicemodestub.h \
    $$STUB_PREFIX/displaystatestub.h \
    $$STUB_PREFIX/batterystub.h \
    $$STUB_PREFIX/ledstub.h \
    $$STUB_PREFIX/gconfstub.h

SOURCES += \
    ut_batterybusinesslogic.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/batterybusinesslogic.cpp \    
    $$STUB_PREFIX/devicemodestub.cpp \
    $$STUB_PREFIX/displaystatestub.cpp \
    $$STUB_PREFIX/batterystub.cpp \
    $$STUB_PREFIX/ledstub.cpp \
    $$STUB_PREFIX/gconfstub.cpp

include(../common_bot.pri)
