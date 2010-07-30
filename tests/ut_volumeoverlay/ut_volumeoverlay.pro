include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumeoverlay

SRC_PREFIX = ../../src
INCLUDEPATH += $$SRC_PREFIX \
               $$SRC_PREFIX/volumecontrol \
               /usr/include/resource/qt4

CONFIG += link_pkgconfig


HEADERS += \
    $$SRC_PREFIX/debug.h \
    ut_volumeoverlay.h \
    $$SRC_PREFIX/volumecontrol/volumeoverlay.h


SOURCES += \
    $$SRC_PREFIX/debug.cpp \
    ut_volumeoverlay.cpp \
    $$SRC_PREFIX/volumecontrol/volumeoverlay.cpp

include(../common_bot.pri)
