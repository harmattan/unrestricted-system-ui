include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_date

INCLUDEPATH += $$SRCDIR/lockscreen

# unit test and unit
SOURCES += \
    ut_date.cpp \
    $$SRCDIR/lockscreen/date.cpp

# unit test and unit
HEADERS += \
    ut_date.h \
    $$SRCDIR/lockscreen/date.h \

include(../common_bot.pri)
