include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumebar

INCLUDEPATH += \
               $$ROOTSRCDIR/extensions/volume \
               /usr/include/resource/qt4

CONFIG += link_pkgconfig

STYLE_HEADERS += \
    $$ROOTSRCDIR/extensions/volume/volumebarstyle.h

HEADERS += \
    ut_volumebar.h \
    $$ROOTSRCDIR/extensions/volume/volumebar.h \
    $$STYLE_HEADERS


SOURCES += \
    ut_volumebar.cpp \
    $$ROOTSRCDIR/extensions/volume/volumebar.cpp

include(../common_bot.pri)
