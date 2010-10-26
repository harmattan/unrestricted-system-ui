include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumebar

INCLUDEPATH += \
               $$SRCDIR/volumecontrol \
               /usr/include/resource/qt4

CONFIG += link_pkgconfig

STYLE_HEADERS += \
    $$SRCDIR/volumecontrol/volumebarstyle.h

HEADERS += \
    $$SRCDIR/debug.h \
    ut_volumebar.h \
    $$SRCDIR/volumecontrol/volumebar.h \
    $$STYLE_HEADERS


SOURCES += \
    $$SRCDIR/debug.cpp \
    ut_volumebar.cpp \
    $$SRCDIR/volumecontrol/volumebar.cpp

include(../common_bot.pri)
