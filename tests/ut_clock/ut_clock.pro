include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_clock

INCLUDEPATH += $$SRCDIR/statusarea

MODEL_HEADERS += $$SRCDIR/statusarea/clockmodel.h

# unit test and unit
SOURCES += \
    ut_clock.cpp \
    $$SRCDIR/statusarea/clock.cpp

# unit test and unit
HEADERS += \
    ut_clock.h \
    $$SRCDIR/statusarea/clock.h \
    $$SRCDIR/statusarea/clockmodel.h

include(../common_bot.pri)
