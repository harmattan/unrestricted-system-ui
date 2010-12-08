include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwithoutpadlockview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock $$SRCDIR/statusindicatormenu $$SRCDIR/screenlock

STYLE_HEADERS += $$ROOTSRCDIR/extensions/screenlock/lockscreenstyle.h

SOURCES += ut_lockscreenwithoutpadlockview.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenwithoutpadlockview.cpp  \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwithoutpadlockview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenwithoutpadlockview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenstyle.h \
    $$ROOTSRCDIR/extensions/screenlock/screenlockextension.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h

include(../common_bot.pri)
