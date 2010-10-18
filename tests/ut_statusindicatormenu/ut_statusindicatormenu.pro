include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicatormenu
INCLUDEPATH += $$SRCDIR/statusindicatormenu

# unit test and unit classes
SOURCES += \
    ut_statusindicatormenu.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenu.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_statusindicatormenu.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenu.h

include(../common_bot.pri)
