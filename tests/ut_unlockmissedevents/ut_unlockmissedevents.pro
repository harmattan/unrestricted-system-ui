include(../coverage.pri)
include(../common_top.pri)

TARGET = ut_unlockmissedevents
INCLUDEPATH += $$SRCDIR $$STUBSDIR $$SRCDIR/lockscreen

SOURCES += ut_unlockmissedevents.cpp \
    $$SRCDIR/lockscreen/unlockmissedevents.cpp

HEADERS += ut_unlockmissedevents.h \
    $$SRCDIR/lockscreen/unlockmissedevents.h

include(../common_bot.pri)
