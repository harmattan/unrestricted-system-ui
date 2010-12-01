include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenview

INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock

SOURCES += ut_lockscreenview.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.cpp  \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_lockscreenview.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenview.h

include(../common_bot.pri)

