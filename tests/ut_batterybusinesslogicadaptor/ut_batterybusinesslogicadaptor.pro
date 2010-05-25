include(../common_top.pri)
TARGET = ut_batterybusinesslogicadaptor
INCLUDEPATH += $$SRCDIR

# unit test and unit
SOURCES += \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/batterybusinesslogicadaptor.cpp \
    $$SRCDIR/batterybusinesslogic.cpp \
    $$STUBSDIR/batterystub.cpp \
    $$STUBSDIR/gconfstub.cpp \
    ut_batterybusinesslogicadaptor.cpp

HEADERS += \
    $$SRCDIR/debug.h \
    $$SRCDIR/batterybusinesslogicadaptor.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$STUBSDIR/batterystub.h \
    $$STUBSDIR/gconfstub.h \
    ut_batterybusinesslogicadaptor.h

include(../common_bot.pri)
