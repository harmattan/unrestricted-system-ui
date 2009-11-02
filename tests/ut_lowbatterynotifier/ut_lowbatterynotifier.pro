SLIDER_PREFIX = ../../src/
INCLUDEPATH += $$SLIDER_PREFIX
QT += testlib \
    dbus
TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_lowbatterynotifier
target.path = /usr/lib/system-ui-tests
CONFIG += dui \
    cellular-qt \
    qmsystem
SOURCES += ut_lowbatterynotifier.cpp \
    $$SLIDER_PREFIX/batterybusinesslogic.cpp \
    $$SLIDER_PREFIX/displaystatestub.cpp \
    $$SLIDER_PREFIX/batterystub.cpp \
    $$SLIDER_PREFIX/systemuigconf.cpp

HEADERS += ut_lowbatterynotifier.h \
    $$SLIDER_PREFIX/batterybusinesslogic.h \
    $$SLIDER_PREFIX/displaystatestub.h \
    $$SLIDER_PREFIX/batterystub.h \
    $$SLIDER_PREFIX/systemuigconf.h

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}


INSTALLS += target
