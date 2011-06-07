include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusareaview
INCLUDEPATH += $$SRCDIR/statusarea $$SRCDIR/statusindicatormenu $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR
INCLUDEPATH += /usr/include/contextsubscriber

STYLE_HEADERS += $$SRCDIR/statusarea/statusareastyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit
SOURCES += \
    ut_statusareaview.cpp \
    $$SRCDIR/statusarea/statusareaview.cpp \
    $$SRCDIR/statusarea/statusarea.cpp

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusareaview.h \
    $$SRCDIR/statusarea/statusareastyle.h \
    $$SRCDIR/statusarea/statusareamodel.h \
    $$SRCDIR/statusarea/statusareaview.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusarea/clock.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusarea/notificationstatusindicator.h \
    $$NOTIFICATIONSRCDIR/notifiernotificationsink.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/statusarea/inputmethodstatusindicatoradaptor.h

include(../common_bot.pri)
