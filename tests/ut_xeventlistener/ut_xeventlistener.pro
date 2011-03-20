include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_xeventlistener

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/

SOURCES += ut_xeventlistener.cpp \
    $$SRCDIR/xeventlistener.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_xeventlistener.h \
    $$SRCDIR/xeventlistener.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

