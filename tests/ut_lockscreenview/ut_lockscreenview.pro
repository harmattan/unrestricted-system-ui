include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

SOURCES += ut_lockscreenview.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/lockscreenview.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenview.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/lockscreenview.h

include(../common_bot.pri)

