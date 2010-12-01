include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwithpadlockview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock

SOURCES += ut_lockscreenwithpadlockview.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenwithpadlockview.cpp  \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwithpadlockview.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.h \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenwithpadlockview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.h

include(../common_bot.pri)

