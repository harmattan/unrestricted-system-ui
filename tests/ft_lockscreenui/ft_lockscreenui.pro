include(../common_top.pri)
TARGET = ft_lockscreenui

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

contains(DEFINES, HAVE_QMSYSTEM) {
	PKGCONFIG += qmsystem
}

SOURCES += ft_lockscreenui.cpp \
    xchecker.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/unlockwidgets.cpp \
    $$SRCDIR/lockscreenui.cpp  \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/unlockmissedevents.cpp \
    $$SRCDIR/unlocknotifications.cpp

HEADERS += ft_lockscreenui.h \
    xchecker.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/unlockwidgets.h \
    $$SRCDIR/lockscreenui.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/unlockmissedevents.h \
    $$SRCDIR/unlocknotifications.h

include(../common_bot.pri)

