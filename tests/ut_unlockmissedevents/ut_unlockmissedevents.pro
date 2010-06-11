include(../common_top.pri)

TARGET = ut_unlockmissedevents
INCLUDEPATH += $$SRCDIR $$STUBSDIR

SOURCES += ut_unlockmissedevents.cpp \
    $$SRCDIR/unlockmissedevents.cpp \
    $$SRCDIR/debug.cpp

HEADERS += ut_unlockmissedevents.h \
    $$SRCDIR/unlockmissedevents.h \
    $$SRCDIR/debug.h

include(../common_bot.pri)
