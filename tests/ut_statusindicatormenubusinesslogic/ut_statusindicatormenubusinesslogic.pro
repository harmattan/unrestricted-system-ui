include(../coverage.pri)
include(../common_top.pri)

TARGET = ut_statusindicatormenubusinesslogic
INCLUDEPATH += $$SRCDIR/statusindicatormenu

# unit test and unit
SOURCES += \
    ut_statusindicatormenubusinesslogic.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenubusinesslogic.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusindicatormenubusinesslogic.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenubusinesslogic.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/closeeventeater.h

include(../common_bot.pri)
