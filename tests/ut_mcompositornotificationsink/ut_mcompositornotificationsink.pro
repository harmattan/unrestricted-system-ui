include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_mcompositornotificationsink
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit classes
SOURCES += \
    ut_mcompositornotificationsink.cpp \
    $$NOTIFICATIONSRCDIR/mcompositornotificationsink.cpp \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_mcompositornotificationsink.h \
    $$NOTIFICATIONSRCDIR/mcompositornotificationsink.h \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$STUBSDIR/stubbase.h

include(../common_bot.pri)
