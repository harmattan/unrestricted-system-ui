include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwindow

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

SOURCES += ut_lockscreenwindow.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/unlockwidgets.cpp \
    $$SRCDIR/lockscreenwindow.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwindow.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/unlockwidgets.h \
    $$SRCDIR/lockscreenui.h \
    $$SRCDIR/lockscreenwindow.h  \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/unlockmissedevents.h \
    $$STUBSDIR/unlockmissedevents_stub.h \

include(../common_bot.pri)

