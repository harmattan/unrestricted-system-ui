include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_screenlockwindow

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/screenlock $$SRCDIR/notifications

STYLE_HEADERS += $$SRCDIR/screenlock/screenlockwindowstyle.h

SOURCES += ut_screenlockwindow.cpp \
    $$SRCDIR/screenlock/screenlockwindow.cpp \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_screenlockwindow.h \
    $$SRCDIR/screenlock/screenlockwindow.h \
    $$SRCDIR/screenlock/screenlockwindowstyle.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/notifications/notifiernotificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

