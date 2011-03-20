include(../common_top.pri)
TARGET = ft_shutdownbusinesslogic

PKGCONFIG += xcomposite

INCLUDEPATH += \
    $$SRCDIR \
    $$STUBSDIR \
    $$COMMONDIR

SOURCES += ft_shutdownbusinesslogic.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/shutdownbusinesslogic.cpp \
    $$SRCDIR/shutdownui.cpp \
    $$SRCDIR/xeventlistener.cpp \
    $$SRCDIR/x11wrapper.cpp

HEADERS += ft_shutdownbusinesslogic.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/shutdownbusinesslogic.h \
    $$SRCDIR/shutdownui.h \
    $$SRCDIR/xeventlistener.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)

