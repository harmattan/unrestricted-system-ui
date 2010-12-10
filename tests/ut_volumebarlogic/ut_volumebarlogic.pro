include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumebarlogic
 
INCLUDEPATH += $$SRCDIR/volumecontrol

CONFIG += link_pkgconfig
PKGCONFIG += dbus-1 dbus-glib-1 libresourceqt1 qmsystem2

HEADERS += \
    ut_volumebarlogic.h \
    $$SRCDIR/volumecontrol/volumebarlogic.h \
    $$SRCDIR/volumecontrol/volumebarwindow.h \
    $$SRCDIR/closeeventeater.h

SOURCES += \
    ut_volumebarlogic.cpp \
    $$SRCDIR/volumecontrol/volumebarlogic.cpp \
    $$STUBSDIR/stubbase.cpp

include(../common_bot.pri)
