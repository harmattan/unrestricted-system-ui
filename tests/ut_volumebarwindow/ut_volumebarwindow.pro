include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumebarwindow
 
INCLUDEPATH +=  \
               $$LIBNOTIFICATIONSRCDIR \
               $$ROOTSRCDIR/extensions/volume

CONFIG += link_pkgconfig
PKGCONFIG += libresourceqt1

STYLE_HEADERS += $$ROOTSRCDIR/extensions/volume/volumebarstyle.h

HEADERS += \
    ut_volumebarwindow.h \
    $$ROOTSRCDIR/extensions/volume/volumebarwindow.h \
    $$ROOTSRCDIR/extensions/volume/volumebarlogic.h \
    $$ROOTSRCDIR/extensions/volume/volumebar.h \
    $$STYLE_HEADERS

SOURCES += \
    ut_volumebarwindow.cpp \
    $$ROOTSRCDIR/extensions/volume/volumebarwindow.cpp \
    $$STUBSDIR/stubbase.cpp

include(../common_bot.pri)
