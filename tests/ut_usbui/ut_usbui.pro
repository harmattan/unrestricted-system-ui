include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_usbui
INCLUDEPATH += $$SRCDIR

contains(DEFINES, HAVE_QMSYSTEM) {
	PKGCONFIG += qmsystem2
}

# unit test and unit
SOURCES += \
    $$SRCDIR/usbui.cpp \
    $$STUBSDIR/locksstub.cpp \
    ut_usbui.cpp

HEADERS += \
    $$SRCDIR/usbui.h \
    $$STUBSDIR/usbmode_stub.h \
    $$STUBSDIR/locksstub.h \
    ut_usbui.h

include(../common_bot.pri)
