include(../common_top.pri)
TARGET = ft_lockdbusinterface

SOURCES += ft_lockdbusinterface.cpp \
    ../ft_lockscreenui/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/debug.cpp

HEADERS += ft_lockdbusinterface.h \
    ../ft_lockscreenui/xchecker.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/debug.h

include(../common_bot.pri)

