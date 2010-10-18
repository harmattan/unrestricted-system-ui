include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusareaview
INCLUDEPATH += $$SRCDIR/statusarea $$SRCDIR/statusindicatormenu $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR
INCLUDEPATH += /usr/include/contextsubscriber

STYLE_HEADERS += $$SRCDIR/statusarea/statusareastyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit
SOURCES += \
    $$SRCDIR/debug.cpp \
    ut_statusareaview.cpp \
    $$SRCDIR/statusarea/statusareaview.cpp \
    $$SRCDIR/statusarea/statusarea.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.cpp

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    $$SRCDIR/debug.h \
    ut_statusareaview.h \
    $$SRCDIR/statusarea/statusareastyle.h \
    $$SRCDIR/statusarea/statusareamodel.h \
    $$SRCDIR/statusarea/statusareaview.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusarea/clock.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuadaptor.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenu.h \
   $$NOTIFICATIONSRCDIR/notificationstatusindicator.h

# service classes
HEADERS += \
    $$STUBSDIR/statusindicatormenuadaptor_stub.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h

include(../common_bot.pri)
