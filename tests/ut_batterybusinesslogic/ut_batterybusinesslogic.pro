include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_batterybusinesslogic

CONFIG -= qmsystem2

QMSYSTEMINCLUDEPATH = /usr/include/qmsystem2

INCLUDEPATH += $$QMSYSTEMINCLUDEPATH


HEADERS += \
    ut_batterybusinesslogic.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$QMSYSTEMINCLUDEPATH/qmled.h \
    $$QMSYSTEMINCLUDEPATH/qmbattery.h \
    $$QMSYSTEMINCLUDEPATH/qmdisplaystate.h \
    $$QMSYSTEMINCLUDEPATH/qmdevicemode.h \
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
