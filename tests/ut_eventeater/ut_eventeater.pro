include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_eventeater

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen

SOURCES += ut_eventeater.cpp \
    $$SRCDIR/lockscreen/eventeater.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_eventeater.h \
    $$SRCDIR/lockscreen/eventeater.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

