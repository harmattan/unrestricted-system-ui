include(../common_top.pri)
TARGET = ft_shutdownui

 SOURCES += ft_shutdownui.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/shutdownui.cpp 

HEADERS += ft_shutdownui.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/shutdownui.h 

include(../common_bot.pri)

