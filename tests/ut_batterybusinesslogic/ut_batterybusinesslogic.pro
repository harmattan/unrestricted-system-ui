SRC_PREFIX = ../../components/sysuid
STUB_PREFIX = ../stubs/
INCLUDEPATH += $$SRC_PREFIX \
    $$STUB_PREFIX
QT += testlib
TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_batterybusinesslogic
target.path = /usr/lib/system-ui-tests
CONFIG += dui silent 

HEADERS += \
    ut_batterybusinesslogic.h \
    $$SRC_PREFIX/../debug.h \
    $$SRC_PREFIX/batterybusinesslogic.h \    
    $$STUB_PREFIX/devicemodestub.h \
    $$STUB_PREFIX/displaystatestub.h \
    $$STUB_PREFIX/batterystub.h \
    $$STUB_PREFIX/ledstub.h \
    $$STUB_PREFIX/gconfstub.h

SOURCES += \
    ut_batterybusinesslogic.cpp \
    $$SRC_PREFIX/../debug.cpp \
    $$SRC_PREFIX/batterybusinesslogic.cpp \    
    $$STUB_PREFIX/devicemodestub.cpp \
    $$STUB_PREFIX/displaystatestub.cpp \
    $$STUB_PREFIX/batterystub.cpp \
    $$STUB_PREFIX/ledstub.cpp \
    $$STUB_PREFIX/gconfstub.cpp

INSTALLS += target
