include(../coverage.pri)
include(../common_top.pri)

TARGET = ut_unlockmissedevents
INCLUDEPATH += $$SRCDIR $$STUBSDIR $$ROOTSRCDIR/extensions/screenlock

SOURCES += ut_unlockmissedevents.cpp \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.cpp

HEADERS += ut_unlockmissedevents.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.h

include(../common_bot.pri)
