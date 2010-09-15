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
    $$NOTIFICATIONSRCDIR/mnotificationproxy.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_mcompositornotificationsink.h \
    $$NOTIFICATIONSRCDIR/mcompositornotificationsink.h \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
    $$NOTIFICATIONSRCDIR/mnotificationproxy.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$STUBSDIR/stubbase.h

include(../common_bot.pri)
