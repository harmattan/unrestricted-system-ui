include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreen

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen

SOURCES += ut_lockscreen.cpp \
    $$SRCDIR/lockscreen/lockscreen.cpp  \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreen.h \
    $$SRCDIR/lockscreen/lockscreen.h \

include(../common_bot.pri)

