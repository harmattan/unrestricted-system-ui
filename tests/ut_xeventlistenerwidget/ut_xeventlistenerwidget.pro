include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_xeventlistenerwidget

SOURCES += ut_xeventlistenerwidget.cpp \
    $$SRCDIR/xeventlistenerwidget.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_xeventlistenerwidget.h \
    $$SRCDIR/xeventlistenerwidget.h \
    $$SRCDIR/xeventlistener.h

include(../common_bot.pri)
