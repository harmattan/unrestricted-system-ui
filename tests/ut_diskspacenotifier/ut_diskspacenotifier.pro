include(../common_top.pri)
TARGET = ut_diskspacenotifier

# unit test and unit
SOURCES += \
    ut_diskspacenotifier.cpp \
    $$SRCDIR/diskspacenotifier.cpp

# unit test and unit
HEADERS += \
    ut_diskspacenotifier.h \
    $$SRCDIR/diskspacenotifier.h

include(../common_bot.pri)
