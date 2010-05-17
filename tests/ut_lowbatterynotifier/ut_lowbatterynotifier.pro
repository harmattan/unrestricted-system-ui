include(../common_top.pri)
TARGET = ut_lowbatterynotifier

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/batterybusinesslogic.cpp \
    $$STUBSDIR/displaystatestub.cpp \
    $$STUBSDIR/batterystub.cpp \
    $$STUBSDIR/gconfstub.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$STUBSDIR/displaystatestub.h \
    $$STUBSDIR/batterystub.h \
    $$STUBSDIR/gconfstub.h

include(../common_bot.pri)
