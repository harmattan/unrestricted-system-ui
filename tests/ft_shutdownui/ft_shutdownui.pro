include(../common_top.pri)
TARGET = ft_shutdownui

INCLUDEPATH += $$COMMONDIR

SOURCES += ft_shutdownui.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/shutdownui.cpp 

HEADERS += ft_shutdownui.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/shutdownui.h 

include(../common_bot.pri)

