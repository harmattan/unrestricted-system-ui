include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_eventeater

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

SOURCES += ut_eventeater.cpp \
    $$SRCDIR/eventeater.cpp \
    $$SRCDIR/debug.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_eventeater.h \
    $$SRCDIR/eventeater.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/lockscreenui.h

include(../common_bot.pri)

