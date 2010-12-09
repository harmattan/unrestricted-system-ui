include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwithpadlockview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock $$SRCDIR/screenlock $$SRCDIR/notifications

STYLE_HEADERS += $$ROOTSRCDIR/extensions/screenlock/lockscreenstyle.h

SOURCES += ut_lockscreenwithpadlockview.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenwithpadlockview.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreenwithpadlockview.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.h \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.h \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotificationsink.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenwithpadlockview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenstyle.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenheader.h \
    $$ROOTSRCDIR/extensions/screenlock/screenlockextension.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h

include(../common_bot.pri)
