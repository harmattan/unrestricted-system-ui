include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenstatusareaview
INCLUDEPATH += $$SRCDIR/statusarea $$ROOTSRCDIR/extensions/screenlock $$SRCDIR/screenlock $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR
INCLUDEPATH += /usr/include/contextsubscriber

STYLE_HEADERS += $$ROOTSRCDIR/extensions/screenlock/lockscreenstatusareastyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit
SOURCES += \
    ut_lockscreenstatusareaview.cpp \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenstatusareaview.cpp \
    $$SRCDIR/statusarea/statusarea.cpp \

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_lockscreenstatusareaview.h \
    $$SRCDIR/statusarea/statusareamodel.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenstatusareastyle.h \
    $$ROOTSRCDIR/extensions/screenlock/lockscreenstatusareaview.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusarea/clock.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$SRCDIR/statusarea/notificationstatusindicator.h \
    $$ROOTSRCDIR/extensions/screenlock/screenlockextension.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)
