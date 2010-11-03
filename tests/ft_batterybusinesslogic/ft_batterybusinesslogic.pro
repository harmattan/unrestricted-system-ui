include(../coverage.pri)
include(../common_top.pri)
TARGET = ft_batterybusinesslogic

INCLUDEPATH += $$COMMONDIR

HEADERS += \
    ft_batterybusinesslogic.h \
    $$COMMONDIR/debug.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$STUBSDIR/qmled_stub.h \
    $$STUBSDIR/qmbattery_stub.h \
    $$STUBSDIR/qmdevicemode_stub.h \
    $$STUBSDIR/stubbase.h

SOURCES += \
    ft_batterybusinesslogic.cpp \
    $$COMMONDIR/debug.cpp \
    $$SRCDIR/batterybusinesslogic.cpp \
    $$STUBSDIR/stubbase.cpp

include(../common_bot.pri)
