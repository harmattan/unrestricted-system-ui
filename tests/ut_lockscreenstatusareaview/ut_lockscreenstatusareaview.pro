include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_lockscreenstatusareaview
INCLUDEPATH += $$SRCDIR/statusarea $$SRCDIR/lockscreen $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR
INCLUDEPATH += /usr/include/contextsubscriber

STYLE_HEADERS += $$SRCDIR/lockscreen/lockscreenstatusareastyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit
SOURCES += \
    ut_lockscreenstatusareaview.cpp \
    $$SRCDIR/lockscreen/lockscreenstatusareaview.cpp \
    $$SRCDIR/statusarea/statusarea.cpp \

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    $$SRCDIR/debug.h \
    ut_lockscreenstatusareaview.h \
    $$SRCDIR/statusarea/statusareamodel.h \
    $$SRCDIR/lockscreen/lockscreenstatusareastyle.h \
    $$SRCDIR/lockscreen/lockscreenstatusareaview.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusarea/clock.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$SRCDIR/notifications/notificationstatusindicator.h \
    $$SRCDIR/notifications/notifiernotificationsink.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)
