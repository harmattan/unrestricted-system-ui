SRC_PREFIX = ../../src/
STUB_PREFIX = ../stubs/
INCLUDEPATH += $$SRC_PREFIX \
    $$STUB_PREFIX \
    /usr/include/dbus-1.0 \
    /usr/lib/dbus-1.0/include
QT += dbus \
    testlib
LIBS += -lcallui0
TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_pincodequerybusinesslogic
target.path = /usr/lib/system-ui-tests
CONFIG += dui
SOURCES += ut_pincodequerybusinesslogic.cpp \
    $$SRC_PREFIX/pincodequerybusinesslogic.cpp \
    $$SRC_PREFIX/pincodequeryui.cpp \
    pinstubs.cpp
HEADERS += ut_pincodequerybusinesslogic.h \
    $$SRC_PREFIX/pincodequerybusinesslogic.h \
    $$SRC_PREFIX/pincodequeryui.h \
    pinstubs.h
INSTALLS += target
