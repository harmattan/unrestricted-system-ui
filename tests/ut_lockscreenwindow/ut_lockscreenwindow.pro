include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenwindow

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen

SOURCES += ut_lockscreenwindow.cpp \
    $$SRCDIR/lockscreen/lockscreenwindow.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenwindow.h \
    $$SRCDIR/lockscreen/lockscreenwindow.h  \
    $$SRCDIR/lockscreen/lockscreen.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

