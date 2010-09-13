include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenui

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

SOURCES += ut_lockscreenui.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/unlockwidgets.cpp \
    $$SRCDIR/lockscreenui.cpp  \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/unlocknotifications.cpp

HEADERS += ut_lockscreenui.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/unlockwidgets.h \
    $$SRCDIR/lockscreenui.h \
    $$SRCDIR/lockscreenwindow.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/unlockmissedevents.h \
    $$STUBSDIR/unlockmissedevents_stub.h \
    $$SRCDIR/unlocknotifications.h

include(../common_bot.pri)

