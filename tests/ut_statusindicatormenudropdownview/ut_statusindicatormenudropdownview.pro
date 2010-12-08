include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicatormenudropdownview
INCLUDEPATH += $$SRCDIR/statusindicatormenu $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit classes
SOURCES += \
    ut_statusindicatormenudropdownview.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenudropdownview.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenu.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_statusindicatormenudropdownview.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenudropdownview.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenu.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h \
    $$SRCDIR/sysuid.h

include(../common_bot.pri)
