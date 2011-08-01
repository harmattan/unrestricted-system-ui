include(../common_top.pri)
TARGET = ut_lowmemorynotifier
PKGCONFIG += libmemnotifyqt

# unit test and unit
SOURCES += \
    ut_lowmemorynotifier.cpp \
    $$SRCDIR/lowmemorynotifier.cpp

# unit test and unit
HEADERS += \
    ut_lowmemorynotifier.h \
    $$SRCDIR/lowmemorynotifier.h

include(../common_bot.pri)
