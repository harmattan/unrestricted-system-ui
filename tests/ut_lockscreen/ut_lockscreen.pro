include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreen

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock

SOURCES += ut_lockscreen.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.cpp  \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_lockscreen.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreen.h \

include(../common_bot.pri)

