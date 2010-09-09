include(../common_top.pri)
TARGET = ft_shutdownbusinesslogic

 
 

INCLUDEPATH += \
    $$SRCDIR \
    $$STUBSDIR

SOURCES += ft_shutdownbusinesslogic.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/shutdownbusinesslogic.cpp \
    $$SRCDIR/shutdownui.cpp 

HEADERS += ft_shutdownbusinesslogic.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/shutdownbusinesslogic.h \
    $$SRCDIR/shutdownui.h 

include(../common_bot.pri)

