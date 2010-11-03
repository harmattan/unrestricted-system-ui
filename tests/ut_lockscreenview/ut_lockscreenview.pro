include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen

SOURCES += ut_lockscreenview.cpp \
    $$SRCDIR/lockscreen/lockscreenview.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenview.h \
    $$SRCDIR/lockscreen/lockscreenview.h

include(../common_bot.pri)

