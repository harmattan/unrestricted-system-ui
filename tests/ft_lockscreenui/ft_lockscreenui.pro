include(../common_top.pri)
TARGET = ft_lockscreenui
PKGCONFIG += xcomposite

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen

contains(DEFINES, HAVE_QMSYSTEM) {
	PKGCONFIG += qmsystem
}

SOURCES += ft_lockscreenui.cpp \
    xchecker.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/lockscreen/unlockwidgets.cpp \
    $$SRCDIR/lockscreen/lockscreen.cpp  \
    $$SRCDIR/lockscreen/lockscreenwindow.cpp  \
    $$SRCDIR/lockscreen/eventeater.cpp  \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/lockscreen/unlockmissedevents.cpp \
    $$SRCDIR/lockscreen/unlocknotifications.cpp \
    $$SRCDIR/x11wrapper.cpp

HEADERS += ft_lockscreenui.h \
    xchecker.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/lockscreen/unlockwidgets.h \
    $$SRCDIR/lockscreen/lockscreen.h \
    $$SRCDIR/lockscreen/lockscreenwindow.h  \
    $$SRCDIR/lockscreen/eventeater.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/lockscreen/unlockmissedevents.h \
    $$SRCDIR/lockscreen/unlocknotifications.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

