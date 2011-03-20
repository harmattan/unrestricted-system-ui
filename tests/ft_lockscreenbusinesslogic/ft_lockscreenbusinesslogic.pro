include(../common_top.pri)
TARGET = ft_lockscreenbusinesslogic
PKGCONFIG += xcomposite

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock $$SRCDIR/screenlock $$SRCDIR/notifications $$COMMONDIR

STYLE_HEADERS += $$SRCDIR/screenlock/screenlockwindowstyle.h

SOURCES += ft_lockscreenbusinesslogic.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$SRCDIR/screenlock/screenlockbusinesslogic.cpp \
    $$SRCDIR/screenlock/screenlockwindow.cpp \
    $$SRCDIR/screenlock/eventeater.cpp  \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.cpp \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.cpp \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/x11wrapper.cpp \
    $$SRCDIR/xeventlistener.cpp \
    $$SRCDIR/closeeventeater.cpp

HEADERS += ft_lockscreenbusinesslogic.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/screenlock/screenlockbusinesslogic.h \
    $$SRCDIR/screenlock/screenlockwindow.h \
    $$SRCDIR/screenlock/screenlockwindowstyle.h \
    $$SRCDIR/screenlock/eventeater.h  \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.h \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/x11wrapper.h \
    $$SRCDIR/xeventlistener.h \
    $$SRCDIR/closeeventeater.h

include(../common_bot.pri)

