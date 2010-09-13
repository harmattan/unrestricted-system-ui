include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_widgetnotificationsink
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_widgetnotificationsink.cpp \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \

# helper classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_widgetnotificationsink.h \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h

include(../common_bot.pri)
