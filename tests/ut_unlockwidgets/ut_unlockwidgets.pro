include(../coverage.pri)
include(../common_top.pri)

SRC_PREFIX = ../../src
TARGET = ut_unlockwidgets

# unit test and unit
SOURCES += \
    ut_unlockwidgets.cpp \
    $$SRC_PREFIX/unlockwidgets.cpp \
    $$SRC_PREFIX/debug.cpp


# unit test and unit
HEADERS += \
    ut_unlockwidgets.h \
    $$SRC_PREFIX/unlockwidgets.h \
    $$SRC_PREFIX/debug.h

include(../common_bot.pri)
