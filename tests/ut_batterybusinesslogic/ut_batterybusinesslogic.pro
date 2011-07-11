include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_batterybusinesslogic

HEADERS += \
    ut_batterybusinesslogic.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$SRCDIR/lowbatterynotifier.h \
    $$STUBSDIR/qmled_stub.h \
    $$STUBSDIR/qmbattery_stub.h \
    $$STUBSDIR/qmdevicemode_stub.h \
    $$STUBSDIR/qmdisplaystate_stub.h \
    $$STUBSDIR/stubbase.h

SOURCES += \
    ut_batterybusinesslogic.cpp \
    $$SRCDIR/batterybusinesslogic.cpp \
    $$STUBSDIR/stubbase.cpp

include(../common_bot.pri)
