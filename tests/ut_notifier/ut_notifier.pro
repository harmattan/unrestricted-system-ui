include(../common_top.pri)
TARGET = ut_notifier
INCLUDEPATH += $$SRCDIR/statusarea

# unit test and unit
SOURCES += \
    ut_notifier.cpp \
    $$SRCDIR/statusarea/notifier.cpp

# base classes
SOURCES += \

# helper classes
SOURCES += \
    $$STUBSDIR\stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notifier.h \
    $$SRCDIR/statusarea/notifier.h

include(../common_bot.pri)
