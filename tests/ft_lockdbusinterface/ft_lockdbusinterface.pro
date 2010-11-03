include(../common_top.pri)
TARGET = ft_lockdbusinterface
INCLUDEPATH += $$COMMONDIR

SOURCES += ft_lockdbusinterface.cpp \
    $$COMMONDIR/xchecker.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ft_lockdbusinterface.h \
    $$COMMONDIR/xchecker.h \
    $$SRCDIR/sysuid.h

include(../common_bot.pri)

