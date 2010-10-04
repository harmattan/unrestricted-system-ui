include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationareaview
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$SRCDIR/statusindicatormenu $$LIBNOTIFICATIONSRCDIR

STYLE_HEADERS += $$SRCDIR/statusindicatormenu/notificationareastyle.h
MODEL_HEADERS += $$SRCDIR/statusindicatormenu/notificationareamodel.h

# unit test and unit
SOURCES += \
    ut_notificationareaview.cpp \
    $$SRCDIR/statusindicatormenu/notificationareaview.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_notificationareaview.h \
    $$SRCDIR/statusindicatormenu/notificationareaview.h \
    $$SRCDIR/statusindicatormenu/notificationareastyle.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h \
    $$SRCDIR/statusindicatormenu/notificationareamodel.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h

# base classes
HEADERS += \

# service classes
SOURCES += ../stubs/stubbase.cpp

# service classes
HEADERS += \


include(../common_bot.pri)
