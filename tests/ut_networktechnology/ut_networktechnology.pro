SRC_PREFIX = ../../networkapplet/
STUB_PREFIX = ../stubs/
INCLUDEPATH += $$SRC_PREFIX \
    $$STUB_PREFIX \
    /usr/include/cellular-qt
QT += testlib
TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_networktechnology
target.path = /usr/lib/system-ui-tests
CONFIG += dui \
    cellular-qt
SOURCES += ut_networktechnology.cpp \
    $$SRC_PREFIX/networktechnology.cpp \
    $$STUB_PREFIX/radioaccessstub.cpp \    
    $$STUB_PREFIX/networkcellstub.cpp
HEADERS += ut_networktechnology.h \
    $$SRC_PREFIX/networktechnology.h \
    $$STUB_PREFIX/radioaccessstub.h \    
    $$STUB_PREFIX/networkcellstub.h
INSTALLS += target
