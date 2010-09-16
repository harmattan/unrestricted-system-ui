include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwindow

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

SOURCES += ut_lockscreenwindow.cpp \
    $$SRCDIR/lockscreenwindow.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwindow.h \
    $$SRCDIR/lockscreenwindow.h  \
    $$SRCDIR/lockscreen.h

include(../common_bot.pri)

