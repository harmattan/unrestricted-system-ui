include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_eventeater

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/screenlock

SOURCES += ut_eventeater.cpp \
    $$SRCDIR/screenlock/eventeater.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_eventeater.h \
    $$SRCDIR/screenlock/eventeater.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

