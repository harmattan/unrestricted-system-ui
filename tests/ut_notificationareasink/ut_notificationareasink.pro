include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationareasink
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit classes
SOURCES += ut_notificationareasink.cpp \
    $$NOTIFICATIONSRCDIR/notificationareasink.cpp \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

# helper classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += ut_notificationareasink.h \
    $$NOTIFICATIONSRCDIR/notificationareasink.h \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h

include(../common_bot.pri)
