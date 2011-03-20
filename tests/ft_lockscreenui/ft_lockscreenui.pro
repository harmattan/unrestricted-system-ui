include(../common_top.pri)
TARGET = ft_lockscreenui
PKGCONFIG += xcomposite

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$SRCDIR/screenlock $$ROOTSRCDIR/extensions/screenlock $$SRCDIR/notifications $$COMMONDIR

contains(DEFINES, HAVE_QMSYSTEM) {
	PKGCONFIG += qmsystem2
}

STYLE_HEADERS += $$SRCDIR/screenlock/screenlockwindowstyle.h

SOURCES += ft_lockscreenui.cpp \
    $$SRCDIR/screenlock/screenlockwindow.cpp  \
    $$SRCDIR/screenlock/eventeater.cpp  \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.cpp  \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.cpp \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$SRCDIR/x11wrapper.cpp \
    $$SRCDIR/xeventlistener.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ft_lockscreenui.h \
    $$SRCDIR/screenlock/screenlockwindow.h \
    $$SRCDIR/screenlock/screenlockwindowstyle.h \
    $$SRCDIR/screenlock/eventeater.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.h \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.h \
    $$SRCDIR/sysuid.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/x11wrapper.h \
    $$SRCDIR/xeventlistener.h

include(../common_bot.pri)

