include(../common_top.pri)
TARGET = ut_pluginlist

INCLUDEPATH += $$SRCDIR/statusindicatormenu

# unit test and unit
SOURCES += \
    $$SRCDIR/debug.cpp \
    ut_pluginlist.cpp \
    $$SRCDIR/statusindicatormenu/pluginlist.cpp

# unit test and unit
HEADERS += \
    $$SRCDIR/debug.h \
    ut_pluginlist.h \
    $$SRCDIR/statusindicatormenu/pluginlist.h \


# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# service classes
HEADERS += \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h

include(../common_bot.pri)
