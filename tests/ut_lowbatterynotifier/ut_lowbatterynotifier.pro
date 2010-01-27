SRC_PREFIX = ../../components/sysuid
STUB_PREFIX = ../stubs/
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

QT += testlib \
      dbus

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_lowbatterynotifier
target.path = /usr/lib/system-ui-tests

CONFIG += dui \
          qmsystem \
          silent

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$SRC_PREFIX/../debug.cpp \
    $$SRC_PREFIX/batterybusinesslogic.cpp \
    $$STUB_PREFIX/displaystatestub.cpp \
    $$STUB_PREFIX/batterystub.cpp \
    $$STUB_PREFIX/gconfstub.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$SRC_PREFIX/../debug.h \
    $$SRC_PREFIX/batterybusinesslogic.h \
    $$STUB_PREFIX/displaystatestub.h \
    $$STUB_PREFIX/batterystub.h \
    $$STUB_PREFIX/gconfstub.h

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}


INSTALLS += target
