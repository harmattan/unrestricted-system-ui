include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_volumebarwindow

 
INCLUDEPATH +=  \
               $$LIBNOTIFICATIONSRCDIR \
               $$SRCDIR/volumecontrol \
               /usr/include/resource/qt4

CONFIG += link_pkgconfig

PKGCONFIG += libresourceqt1
PKGCONFIG += qmsystem2

STYLE_HEADERS += \
    $$SRCDIR/volumecontrol/volumebarstyle.h

HEADERS += \
    ut_volumebarwindow.h \
    $$SRCDIR/volumecontrol/volumebarwindow.h \
    $$SRCDIR/volumecontrol/volumebarlogic.h \
    $$SRCDIR/volumecontrol/volumebar.h \
    $$STYLE_HEADERS


SOURCES += \
    ut_volumebarwindow.cpp \
    $$SRCDIR/volumecontrol/volumebarwindow.cpp

include(../common_bot.pri)
