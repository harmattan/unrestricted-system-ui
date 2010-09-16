include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreen

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

SOURCES += ut_lockscreen.cpp \
    $$SRCDIR/lockscreen.cpp  \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreen.h \
    $$SRCDIR/lockscreen.h \

include(../common_bot.pri)

