include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwithoutpadlockview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen $$SRCDIR/statusindicatormenu

SOURCES += ut_lockscreenwithoutpadlockview.cpp \
    $$SRCDIR/lockscreen/lockscreenwithoutpadlockview.cpp  \
    $$SRCDIR/lockscreen/lockscreenview.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwithoutpadlockview.h \
    $$SRCDIR/lockscreen/lockscreenwithoutpadlockview.h \
    $$SRCDIR/lockscreen/lockscreenview.h \
    $$SRCDIR/lockscreen/lockscreen.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h

include(../common_bot.pri)

