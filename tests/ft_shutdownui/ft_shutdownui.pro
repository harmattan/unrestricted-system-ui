include(../common_top.pri)
TARGET = ft_shutdownui

PKGCONFIG += xcomposite

INCLUDEPATH += $$COMMONDIR

SOURCES += ft_shutdownui.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/shutdownui.cpp \
    $$SRCDIR/x11wrapper.cpp

HEADERS += ft_shutdownui.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/shutdownui.h \
    $$SRCDIR/shutdownwindowstyle.h \
    $$SRCDIR/x11wrapper.h

STYLE_HEADERS += $$SRCDIR/shutdownwindowstyle.h

include(../common_bot.pri)

