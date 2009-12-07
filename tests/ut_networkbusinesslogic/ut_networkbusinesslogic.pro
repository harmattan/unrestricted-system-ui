SRC_PREFIX = ../../networkapplet/
STUB_PREFIX = ../stubs/
INCLUDEPATH += $$SRC_PREFIX \
    $$STUB_PREFIX \
    /usr/include/cellular-qt
QT += testlib 
TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_networkbusinesslogic
target.path = /usr/lib/system-ui-tests
CONFIG += dui \
    cellular-qt
SOURCES += ut_networkbusinesslogic.cpp \
    $$SRC_PREFIX/networkbusinesslogic.cpp \
    $$SRC_PREFIX/networktechnology.cpp \
    $$SRC_PREFIX/systemuigconf.cpp \
    $$STUB_PREFIX/networkregistrationstub.cpp \
    $$STUB_PREFIX/radioaccessstub.cpp \    
    $$STUB_PREFIX/networkoperatorstub.cpp
HEADERS += ut_networkbusinesslogic.h \
    $$SRC_PREFIX/networkbusinesslogic.h \
    $$SRC_PREFIX/networktechnology.h \
    $$SRC_PREFIX/systemuigconf.h \
    $$STUB_PREFIX/networkregistrationstub.h \
    $$STUB_PREFIX/radioaccessstub.h \    
    $$STUB_PREFIX/networkoperatorstub.h
INSTALLS += target
