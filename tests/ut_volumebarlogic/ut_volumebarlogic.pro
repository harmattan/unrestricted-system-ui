include(../common_top.pri)
TARGET = ut_volumebarlogic

SRC_PREFIX = ../../src
INCLUDEPATH += $$SRC_PREFIX \
               $$SRC_PREFIX/volumecontrol

PKGCONFIG += dbus-1 dbus-glib-1

INCLUDEPATH += \
    /usr/include/dbus-1.0

HEADERS += \
    $$SRC_PREFIX/debug.h \
    ut_volumebarlogic.h \
    $$SRC_PREFIX/volumecontrol/volumebarlogic.h

SOURCES += \
    $$SRC_PREFIX/debug.cpp \
    ut_volumebarlogic.cpp \
    $$SRC_PREFIX/volumecontrol/volumebarlogic.cpp

include(../common_bot.pri)
