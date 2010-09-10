include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_ngfnotificationsink
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit
SOURCES += \
    ut_ngfnotificationsink.cpp \
   $$NOTIFICATIONSRCDIR/ngfnotificationsink.cpp \
   $$LIBNOTIFICATIONSRCDIR/notification.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp

# base classes
SOURCES += \
   $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_ngfnotificationsink.h \
   $$NOTIFICATIONSRCDIR/ngfnotificationsink.h \

# base classes
HEADERS += \
   $$LIBNOTIFICATIONSRCDIR/notificationsink.h \

# service classes
HEADERS += \
   $$LIBNOTIFICATIONSRCDIR/notification.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
   $$LIBNOTIFICATIONSRCDIR/notification.h \
   $$NOTIFICATIONSRCDIR/ngfadapter.h \
   $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
   $$NOTIFICATIONSRCDIR/notificationmanager.h \
   $$NOTIFICATIONSRCDIR/eventtypestore.h \
    $$SRCDIR/sysuid.h

include(../common_bot.pri)
