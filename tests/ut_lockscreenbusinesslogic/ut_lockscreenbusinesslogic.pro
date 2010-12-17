include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenbusinesslogic
INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/screenlock $$SRCDIR/notifications

SOURCES += ut_lockscreenbusinesslogic.cpp \
    $$SRCDIR/screenlock/screenlockbusinesslogic.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreenbusinesslogic.h \
    $$SRCDIR/screenlock/screenlockbusinesslogic.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/screenlock/screenlockwindow.h \
    $$SRCDIR/screenlock/eventeater.h \
    $$SRCDIR/closeeventeater.h \
    $$SRCDIR/notifications/notifiernotificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h

include(../common_bot.pri)

