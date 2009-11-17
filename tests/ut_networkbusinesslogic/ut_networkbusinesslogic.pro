SRC_PREFIX = ../../networkapplet/
STUB_PREFIX = ../stubs/
INCLUDEPATH += $$SRC_PREFIX \
    $$STUB_PREFIX \
    /usr/include/cellular-qt
QT += testlib \
    dbus
TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_networkbusinesslogic
target.path = /usr/lib/system-ui-tests
CONFIG += dui \
    cellular-qt
SOURCES += ut_networkbusinesslogic.cpp \
    $$SRC_PREFIX/networkbusinesslogic.cpp \
    $$SRC_PREFIX/networkdbusinterface.cpp \
    $$STUB_PREFIX/networkregistrationstub.cpp \
    $$STUB_PREFIX/radioaccessstub.cpp \
    $$STUB_PREFIX/signalstrengthstub.cpp \
    $$STUB_PREFIX/networkoperatorstub.cpp \
    $$STUB_PREFIX/networkcellstub.cpp
HEADERS += ut_networkbusinesslogic.h \
    $$SRC_PREFIX/networkbusinesslogic.h \
    $$SRC_PREFIX/networkdbusinterface.h \
    $$STUB_PREFIX/networkregistrationstub.h \
    $$STUB_PREFIX/radioaccessstub.h \
    $$STUB_PREFIX/signalstrengthstub.h \
    $$STUB_PREFIX/networkoperatorstub.h \
    $$STUB_PREFIX/networkcellstub.h
INSTALLS += target
