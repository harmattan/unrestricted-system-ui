include(../common_top.pri)
TARGET = ft_lockscreenui
PKGCONFIG += xcomposite

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen $$COMMONDIR

contains(DEFINES, HAVE_QMSYSTEM) {
	PKGCONFIG += qmsystem2
}

STYLE_HEADERS += $$SRCDIR/lockscreen/lockscreenwindowstyle.h

SOURCES += ft_lockscreenui.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$SRCDIR/lockscreen/unlockarea.cpp \
    $$SRCDIR/lockscreen/lockscreen.cpp  \
    $$SRCDIR/lockscreen/lockscreenwindow.cpp  \
    $$SRCDIR/lockscreen/eventeater.cpp  \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/lockscreen/unlockmissedevents.cpp \
    $$SRCDIR/lockscreen/unlocknotifications.cpp \
    $$SRCDIR/x11wrapper.cpp

HEADERS += ft_lockscreenui.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/lockscreen/unlockarea.h \
    $$SRCDIR/lockscreen/lockscreen.h \
    $$SRCDIR/lockscreen/lockscreenwindow.h \
    $$SRCDIR/lockscreen/lockscreenwindowstyle.h \
    $$SRCDIR/lockscreen/eventeater.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/lockscreen/unlockmissedevents.h \
    $$SRCDIR/lockscreen/unlocknotifications.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

