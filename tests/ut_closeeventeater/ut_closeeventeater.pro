include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_closeeventeater

# unit test and unit
SOURCES += \
    ut_closeeventeater.cpp \
    $$SRCDIR/closeeventeater.cpp

# unit test and unit
HEADERS += \
    ut_closeeventeater.h \
    $$SRCDIR/closeeventeater.h

include(../common_bot.pri)
