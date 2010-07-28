include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumecontrolui

SRC_PREFIX = ../../src
INCLUDEPATH += $$SRC_PREFIX \
               $$SRC_PREFIX/volumecontrol \
               /usr/include/resource/qt4

CONFIG += link_pkgconfig

PKGCONFIG += libresourceqt1
PKGCONFIG += qmsystem

HEADERS += \
    $$SRC_PREFIX/debug.h \
    ut_volumecontrolui.h \
    $$SRC_PREFIX/volumecontrol/volumecontrolui.h \
    $$SRC_PREFIX/volumecontrol/volumebarlogic.h \
    $$SRC_PREFIX/volumecontrol/volumeoverlay.h


SOURCES += \
    $$SRC_PREFIX/debug.cpp \
    ut_volumecontrolui.cpp \
    $$SRC_PREFIX/volumecontrol/volumecontrolui.cpp

include(../common_bot.pri)
