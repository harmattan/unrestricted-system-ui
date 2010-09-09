include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumeoverlay

INCLUDEPATH += \
               $$SRCDIR/volumecontrol \
               /usr/include/resource/qt4

CONFIG += link_pkgconfig

HEADERS += \
    $$SRCDIR/debug.h \
    ut_volumeoverlay.h \
    $$SRCDIR/volumecontrol/volumeoverlay.h


SOURCES += \
    $$SRCDIR/debug.cpp \
    ut_volumeoverlay.cpp \
    $$SRCDIR/volumecontrol/volumeoverlay.cpp

include(../common_bot.pri)
