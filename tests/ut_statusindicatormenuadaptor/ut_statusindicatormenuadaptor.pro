include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicatormenuadaptor

INCLUDEPATH += $$SRCDIR/statusindicatormenu

#STYLE_HEADERS += $$SRCDIR/statusarea/statusindicatorimagestyle.h \
#    $$SRCDIR/statusarea/statusindicatorlabelstyle.h
#MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit classes
SOURCES += \
    $$SRCDIR/statusindicatormenu/statusindicatormenuadaptor.cpp \
    ut_statusindicatormenuadaptor.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    $$SRCDIR/statusindicatormenu/statusindicatormenuadaptor.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenubusinesslogic.h \
    ut_statusindicatormenuadaptor.h

# service classes
HEADERS += \
    $$STUBSDIR/stubbase.h \

include(../common_bot.pri)
