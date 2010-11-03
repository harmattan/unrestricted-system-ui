include(../common_top.pri)
TARGET = ft_shutdownbusinesslogic

INCLUDEPATH += \
    $$SRCDIR \
    $$STUBSDIR \
    $$COMMONDIR

SOURCES += ft_shutdownbusinesslogic.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/shutdownbusinesslogic.cpp \
    $$SRCDIR/shutdownui.cpp 

HEADERS += ft_shutdownbusinesslogic.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/shutdownbusinesslogic.h \
    $$SRCDIR/shutdownui.h 

include(../common_bot.pri)

