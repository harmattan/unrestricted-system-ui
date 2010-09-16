include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwithpadlockview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen

SOURCES += ut_lockscreenwithpadlockview.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/lockscreen/lockscreenwithpadlockview.cpp  \
    $$SRCDIR/lockscreen/lockscreenview.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwithpadlockview.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/lockscreen/unlockwidgets.h \
    $$SRCDIR/lockscreen/unlocknotifications.h \
    $$SRCDIR/lockscreen/lockscreenwithpadlockview.h \
    $$SRCDIR/lockscreen/lockscreenview.h \
    $$SRCDIR/lockscreen/lockscreen.h

include(../common_bot.pri)

