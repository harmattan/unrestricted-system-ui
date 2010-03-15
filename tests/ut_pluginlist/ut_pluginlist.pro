include(../common_top.pri)
TARGET = ut_pluginlist

INCLUDEPATH += $$SRCDIR/statusindicatormenu $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_pluginlist.cpp \
    $$SRCDIR/statusindicatormenu/pluginlist.cpp

# unit test and unit
HEADERS += \
    ut_pluginlist.h \
    $$SRCDIR/statusindicatormenu/pluginlist.h \


# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# service classes
HEADERS += \
    $$SRCDIR/statusindicatormenu/notificationarea.h

include(../common_bot.pri)
