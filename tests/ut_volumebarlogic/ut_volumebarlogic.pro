include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumebarlogic
 
INCLUDEPATH +=  \
               $$SRCDIR/volumecontrol

PKGCONFIG += dbus-1 dbus-glib-1

INCLUDEPATH += \
    /usr/include/dbus-1.0

HEADERS += \
    $$SRCDIR/debug.h \
    ut_volumebarlogic.h \
    $$SRCDIR/volumecontrol/volumebarlogic.h

SOURCES += \
    $$SRCDIR/debug.cpp \
    ut_volumebarlogic.cpp \
    $$SRCDIR/volumecontrol/volumebarlogic.cpp

include(../common_bot.pri)
