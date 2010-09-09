include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumecontrolui

 
INCLUDEPATH +=  \
               $$LIBNOTIFICATIONSRCDIR \
               $$SRCDIR/volumecontrol \
               /usr/include/resource/qt4

CONFIG += link_pkgconfig

PKGCONFIG += libresourceqt1
PKGCONFIG += qmsystem

HEADERS += \
    $$SRCDIR/debug.h \
    ut_volumecontrolui.h \
    $$SRCDIR/volumecontrol/volumecontrolui.h \
    $$SRCDIR/volumecontrol/volumebarlogic.h \
    $$SRCDIR/volumecontrol/volumeoverlay.h


SOURCES += \
    $$SRCDIR/debug.cpp \
    ut_volumecontrolui.cpp \
    $$SRCDIR/volumecontrol/volumecontrolui.cpp

include(../common_bot.pri)
