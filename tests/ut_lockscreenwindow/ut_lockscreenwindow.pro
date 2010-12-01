include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwindow

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/screenlock

STYLE_HEADERS += $$SRCDIR/screenlock/screenlockwindowstyle.h

SOURCES += ut_lockscreenwindow.cpp \
    $$SRCDIR/screenlock/screenlockwindow.cpp \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwindow.h \
    $$SRCDIR/screenlock/screenlockwindow.h \
    $$SRCDIR/screenlock/screenlockwindowstyle.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

