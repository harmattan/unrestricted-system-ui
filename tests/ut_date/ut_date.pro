include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_date

INCLUDEPATH += $$ROOTSRCDIR/extensions/screenlock

STYLE_HEADERS += $$ROOTSRCDIR/extensions/screenlock/datestyle.h

# unit test and unit
SOURCES += \
    ut_date.cpp \
    $$ROOTSRCDIR/extensions/screenlock/date.cpp

# unit test and unit
HEADERS += \
    ut_date.h \
    $$ROOTSRCDIR/extensions/screenlock/date.h \
    $$ROOTSRCDIR/extensions/screenlock/datestyle.h

include(../common_bot.pri)
