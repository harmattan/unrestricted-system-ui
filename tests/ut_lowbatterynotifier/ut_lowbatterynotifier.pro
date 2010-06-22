include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lowbatterynotifier

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/batterybusinesslogic.cpp \
    $$STUBSDIR/displaystatestub.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$STUBSDIR/displaystatestub.h \
    $$STUBSDIR/stubbase.h \
    $$STUBSDIR/qmbattery_stub.h

include(../common_bot.pri)
