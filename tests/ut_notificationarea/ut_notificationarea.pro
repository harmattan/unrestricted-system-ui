include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationarea
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$SRCDIR/statusindicatormenu $$LIBNOTIFICATIONSRCDIR

MODEL_HEADERS += $$SRCDIR/statusindicatormenu/notificationareamodel.h

# unit test and unit
SOURCES += \
    ut_notificationarea.cpp \
    $$SRCDIR/statusindicatormenu/notificationarea.cpp

# service classes
SOURCES += \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notificationarea.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h \
    $$SRCDIR/statusindicatormenu/notificationareamodel.h

# service classes
HEADERS += \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
    $$NOTIFICATIONSRCDIR/notificationareasink.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h

include(../common_bot.pri)
